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

		if (entity.hasComponent<LightComponent>()) {
			const auto& light = entity.getComponent<LightComponent>();

			setAvailableContainerLight(rpc);
			rpc.light_index = submitLight(tran.center, light);
		}

		if (entity.hasComponent<RenderableComponent>()) {
			auto& renderable = entity.getComponent<RenderableComponent>();

			setAvailableContainerRenderable(rpc, renderable.vertices.size(), renderable.indices.size());
			const size_t index = submitRenderable(renderable, tran);

			renderable.shaderID = index;
			rpc.transform_index = index;

			if (entity.hasComponent<ColorComponent>()) {
				const auto& color = entity.getComponent<ColorComponent>();
				rpc.color_index = submitColor((int32_t)renderable.shaderID, color);
			}
			else if (entity.hasComponent<Texture2DComponent>()) {
				const auto& tex = entity.getComponent<Texture2DComponent>();
				rpc.color_index = submitTexture2D((int32_t)renderable.shaderID, tex);
			}
			else if (entity.hasComponent<TextureCubemapComponent>()) {
				const auto& cube = entity.getComponent<TextureCubemapComponent>();
				rpc.color_index = submitCubemap((int32_t)renderable.shaderID, cube);
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
				rpc.container_index = m_availableContainerIndex;
				GRAPHICS_TRACE("RENDER_PIPELINE: available container is at index {}, returning...", i);
				return;
			}
		}

		m_containers.emplace_back();
		m_availableContainerIndex = m_containers.size() - 1;
		rpc.container_index = m_availableContainerIndex;

		GRAPHICS_INFO("RENDER_PIPELINE: emplaced back new render container, current size {}", m_containers.size());
	}

	void RenderPipeline::setAvailableContainerLight(ecs::RenderPipelineComponent& rpc) {
		for (size_t i = 0; i < m_containers.size(); i++) {
			const size_t currentLightSize = m_containers[i].getLights().size();

			if (currentLightSize + 1 >= 32) { continue; }
			else {
				m_availableContainerIndex = i;
				rpc.container_light_index = m_availableContainerIndex;
				GRAPHICS_TRACE("RENDER_PIPELINE: available container for light is at {}, returning...", i);
				return;
			}
		}
		
		m_containers.emplace_back();
		m_availableContainerIndex = m_containers.size() - 1;
		rpc.container_light_index = m_availableContainerIndex;

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
			auto itBegin = vertices.end() - renderable.vertices.size();
			auto itEnd = vertices.end();
			std::for_each(itBegin, itEnd, [newShapeID = shapeID](Vertex& vertex) {
				vertex.shapeID = newShapeID;
			});
		}

		{ // increase indices for every vertex (batch renderer)
			auto& indices = m_containers[m_availableContainerIndex].m_indices;

			indices.insert(indices.end(), renderable.indices.begin(), renderable.indices.end());
			auto itBegin = indices.end() - renderable.indices.size();
			auto itEnd = indices.end();
			std::for_each(itBegin, itEnd, [extension = indicesMax](uint32_t& indice) {
				indice += extension;
			});
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
		availableContainer->m_lights.push_back({ position, light });

		GRAPHICS_TRACE("RENDER_PIPELINE: submitted light component with its center, current size = {}", 
			availableContainer->m_lights.size());
		
		return availableContainer->m_lights.size() - 1;
	}

	void RenderPipeline::submitCamera(RenderCamera* cam) {
		m_camera = cam;

		GRAPHICS_TRACE("RENDER_PIPELINE: submitted Camera!");
	}

	void RenderPipeline::modifyTransform(const ecs::TransformComponent& transform, size_t containerIndex, size_t transformIndex) {
		m_containers[containerIndex].m_transforms[transformIndex] = transform;

		GRAPHICS_TRACE("RENDER_PIPELINE: modified transform component {} at container {}", transformIndex, containerIndex);
	}

	void RenderPipeline::modifyLight(const maths::vec3& position, const ecs::LightComponent& light, size_t containerIndex, size_t lightIndex) {
		m_containers[containerIndex].m_lights[lightIndex].first = position;
		m_containers[containerIndex].m_lights[lightIndex].second = light;

		GRAPHICS_TRACE("RENDER_PIPELINE: modified light component {} at container {}", lightIndex, containerIndex);
	}

	void RenderPipeline::modifyColor(const ecs::ColorComponent& color, size_t containerIndex, size_t colorIndex) {
		m_containers[containerIndex].m_colors[colorIndex].second = color.texture;

		GRAPHICS_TRACE("RENDER_PIPELINE: modified color component {} at container {}", colorIndex, containerIndex);
	}


}
