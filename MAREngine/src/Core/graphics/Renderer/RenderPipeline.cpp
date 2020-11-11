/**
 *           MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


#include "RenderPipeline.h"
#include "RenderCamera.h"
#include "../GraphicsLogs.h"
#include "../../ecs/Entity/Entity.h"


namespace mar::graphics {


	RenderPipeline* RenderPipeline::s_instance{ nullptr };

	void RenderPipeline::initialize() {
		setCurrentPipeline();

		m_containers.emplace_back();
		m_availableContainerIndex = m_containers.size() - 1;

		GRAPHICS_INFO("RENDER_PIPELINE: initialized!");
	}

	void RenderPipeline::reset() {
		for (auto& container : m_containers) {
			container.reset();
		}

		m_containers.clear();

		initialize();

		GRAPHICS_INFO("RENDER_PIPELINE: all data was resetted!");
	}

	void RenderPipeline::submitEntity(const ecs::Entity& entity) {
		GRAPHICS_INFO("RENDER_PIPELINE: going to submit entity into pipeline");

		using namespace ecs;

		const auto& tran = entity.getComponent<TransformComponent>();
		auto& rpc = entity.getComponent<RenderPipelineComponent>();

		const bool hasColor = entity.hasComponent<ColorComponent>();
		const bool hasTexture2D = entity.hasComponent<Texture2DComponent>();
		const bool hasCubemap = entity.hasComponent<TextureCubemapComponent>();
		const bool hasAnyMaterial = hasColor || hasTexture2D || hasCubemap;
		const bool hasRenderable = entity.hasComponent<RenderableComponent>();

		if (entity.hasComponent<LightComponent>()) {
			const auto& light = entity.getComponent<LightComponent>();

			setAvailableContainerLight(rpc);
			rpc.lightIndex = submitLight(tran.center, light);
		}

		if (hasRenderable && hasAnyMaterial) {
			auto& renderable = entity.getComponent<RenderableComponent>();

			setAvailableContainerRenderable(rpc, renderable.vertices.size(), renderable.indices.size());
			const size_t index = submitRenderable(renderable, tran);

			renderable.shaderID = index;
			rpc.transformIndex = index;

			if (hasColor) {
				const auto& color = entity.getComponent<ColorComponent>();
				rpc.colorIndex = submitColor((int32_t)renderable.shaderID, color);
			}
			else if (hasTexture2D) {
				const auto& tex = entity.getComponent<Texture2DComponent>();
				rpc.colorIndex = submitTexture2D((int32_t)renderable.shaderID, tex);
			}
			else if (hasCubemap) {
				const auto& cube = entity.getComponent<TextureCubemapComponent>();
				rpc.colorIndex = submitCubemap((int32_t)renderable.shaderID, cube);
			}
		}

		GRAPHICS_INFO("RENDER_PIPELINE: submitted entity into pipeline");
	}

	void RenderPipeline::setAvailableContainerRenderable(ecs::RenderPipelineComponent& rpc, size_t verticesToPush, size_t indicesToPush) {
		for (size_t i = 0; i < m_containers.size(); i++) {
			const size_t currentVerticesSize = m_containers[i].getVertices().size() + sizeof(Vertex) / 4;
			const size_t currentIndicesSize = m_containers[i].getIndices().size();
			const size_t currentTransformSize = m_containers[i].getTransforms().size();

			if (currentVerticesSize + verticesToPush >= settings::maxVerticesCount ||
				currentIndicesSize + indicesToPush >= settings::maxIndicesCount ||
				currentTransformSize + 1 >= 32)
			{
				continue;
			}
			else {
				m_availableContainerIndex = i;

				rpc.containerIndex = m_availableContainerIndex;
				GRAPHICS_TRACE("RENDER_PIPELINE: available container is at index {}, returning...", i);
				return;
			}
		}

		m_containers.emplace_back();
		m_availableContainerIndex = m_containers.size() - 1;
		rpc.containerIndex = m_availableContainerIndex;

		GRAPHICS_INFO("RENDER_PIPELINE: emplaced back new render container, current size {}", m_containers.size());
	}

	void RenderPipeline::setAvailableContainerLight(ecs::RenderPipelineComponent& rpc) {
		for (size_t i = 0; i < m_containers.size(); i++) {
			const size_t currentLightSize = m_containers[i].getLights().size();

			if (currentLightSize + 1 >= 32) { continue; }
			else {
				m_availableContainerIndex = i;
				rpc.containerLightIndex = m_availableContainerIndex;

				GRAPHICS_TRACE("RENDER_PIPELINE: available container for light is at {}, returning...", i);
				return;
			}
		}
		
		m_containers.emplace_back();
		m_availableContainerIndex = m_containers.size() - 1;
		rpc.containerLightIndex = m_availableContainerIndex;

		GRAPHICS_INFO("RENDER_PIPELINE: emplaced back new render container (for light), current size {}", m_containers.size());
	}

	size_t RenderPipeline::submitRenderable(const ecs::RenderableComponent& renderable, const ecs::TransformComponent& transform) {
		GRAPHICS_INFO("RENDER_PIPELINE: submitting renderable component");

		auto& indicesMax = m_containers[m_availableContainerIndex].m_indicesMax;
		auto& shapeID = m_containers[m_availableContainerIndex].m_shapeID;
		auto& transforms = m_containers[m_availableContainerIndex].m_transforms;

		{ // set shapeID on every vertex for batch renderer
			auto& vertices = m_containers[m_availableContainerIndex].m_vertices;
			vertices.insert(vertices.end(), renderable.vertices.begin(), renderable.vertices.end());

			auto fromBeginOfInsertedVertices = vertices.end() - renderable.vertices.size();
			auto toItsEnd = vertices.end();
			auto modifyShaderID = [newShapeID = shapeID](Vertex& vertex) {
				vertex.shapeID = newShapeID;
			};

			std::for_each(fromBeginOfInsertedVertices, toItsEnd, modifyShaderID);
		}

		{ // increase indices for every vertex (batch renderer)
			auto& indices = m_containers[m_availableContainerIndex].m_indices;
			indices.insert(indices.end(), renderable.indices.begin(), renderable.indices.end());

			auto fromBeginOfInsertedIndices = indices.end() - renderable.indices.size();
			auto toItsEnd = indices.end();
			auto extendIndices = [extension = indicesMax](uint32_t& indice) {
				indice += extension;
			};

			std::for_each(fromBeginOfInsertedIndices, toItsEnd, extendIndices);
		}

		indicesMax += (renderable.vertices.size() * sizeof(Vertex) / 4) / RenderContainer::getStride();
		shapeID++;

		transforms.push_back(transform.transform);

		MAR_CORE_ASSERT(transforms.size() == shapeID, "transform.size() and shapeID are not equal!");

		GRAPHICS_INFO("RENDER_PIPELINE: submitted renderable component {} --- vert_size = {} indi_size = {}, indicesMax = {}, shaderID = {}", 
			renderable.name, renderable.vertices.size(), renderable.indices.size(), indicesMax, renderable.shaderID);
		
		return transforms.size() - 1;
	}

	size_t RenderPipeline::submitColor(int32_t entityIndex, const ecs::ColorComponent& color) {
		RenderContainer* availableContainer = &m_containers[m_availableContainerIndex];
		availableContainer->m_colors.push_back({ entityIndex, color.texture });
		availableContainer->m_samplerTypes.push_back(0.0f);

		GRAPHICS_TRACE("RENDER_PIPELINE: submitted color component, current size = {}, entity_index = {}",
			availableContainer->m_colors.size(), entityIndex);
		
		return availableContainer->m_colors.size() - 1;
	}

	size_t RenderPipeline::submitTexture2D(int32_t entityIndex, const ecs::Texture2DComponent& texture) {
		RenderContainer* availableContainer = &m_containers[m_availableContainerIndex];
		availableContainer->m_tex2D.push_back({ entityIndex, texture.texture });
		availableContainer->m_samplerTypes.push_back(1.0f);

		GRAPHICS_TRACE("RENDER_PIPELINE: submitted texture2d component, current size = {}, entity_index = {}, texture2D = {}", 
			availableContainer->m_tex2D.size(), entityIndex, texture.texture);

		return availableContainer->m_tex2D.size() - 1;
	}

	size_t RenderPipeline::submitCubemap(int32_t entityIndex, const ecs::TextureCubemapComponent& cubemap) {
		RenderContainer* availableContainer = &m_containers[m_availableContainerIndex];
		availableContainer->m_cubes.push_back({ entityIndex, cubemap.texture });
		availableContainer->m_samplerTypes.push_back(2.0f);

		GRAPHICS_TRACE("RENDER_PIPELINE: submitted texture cubemap component, current size = {}, entity_index = {}, textureCubemap = {}", 
			availableContainer->m_cubes.size(), entityIndex, cubemap.texture);

		return availableContainer->m_cubes.size() - 1;
	}

	size_t RenderPipeline::submitLight(const maths::vec3& position, const ecs::LightComponent& light) {
		RenderContainer* availableContainer = &m_containers[m_availableContainerIndex];

		auto& lightMaterial = availableContainer->m_lights.emplace_back();
		lightMaterial.position = position;
		lightMaterial.ambient = light.ambient;
		lightMaterial.diffuse = light.diffuse;
		lightMaterial.specular = light.specular;
		lightMaterial.linear = light.linear;
		lightMaterial.quadratic = light.quadratic;
		lightMaterial.constant = light.constant;
		lightMaterial.shininess = light.shininess;

		GRAPHICS_TRACE("RENDER_PIPELINE: submitted light component with its center, current size = {}", 
			availableContainer->m_lights.size());
		
		return availableContainer->m_lights.size() - 1;
	}

	void RenderPipeline::submitCamera(RenderCamera* cam) {
		m_camera = cam;

		GRAPHICS_TRACE("RENDER_PIPELINE: submitted Camera!");
	}


}
