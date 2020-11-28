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

		GRAPHICS_INFO("RENDER_PIPELINE: initialized!");
	}

	void RenderPipeline::reset() {
		for (auto& container : m_containers2D) { container.reset(); }
		for (auto& container : m_containersCubemap) { container.reset(); }
		for (auto& light : m_lights) { light.reset(); }

		m_containers2D.clear();
		m_containersCubemap.clear();
		m_lights.clear();

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

			setContainerLight(rpc);
			rpc.lightIndex = submitLight(tran.center, light);
		}

		if (hasRenderable && hasAnyMaterial) {
			auto& renderable = entity.getComponent<RenderableComponent>();
			const auto vertSize = renderable.vertices.size();
			const auto indiSize = renderable.indices.size();

			if (hasColor) {
				setContainerRenderable(MaterialRenderType::COLOR, rpc, vertSize, indiSize);
				rpc.transformIndex = submitRenderable(renderable, tran);

				const auto& color = entity.getComponent<ColorComponent>();
				rpc.colorIndex = submitColor((int32_t)rpc.transformIndex, color);
			}
			else if (hasTexture2D) {
				setContainerRenderable(MaterialRenderType::TEXTURE2D, rpc, vertSize, indiSize);
				rpc.transformIndex = submitRenderable(renderable, tran);

				const auto& tex = entity.getComponent<Texture2DComponent>();
				rpc.colorIndex = submitTexture2D((int32_t)rpc.transformIndex, tex);
			}
			else if (hasCubemap) {
				setContainerRenderable(MaterialRenderType::CUBEMAP, rpc, vertSize, indiSize);
				rpc.transformIndex = submitRenderable(renderable, tran);

				const auto& cube = entity.getComponent<TextureCubemapComponent>();
				rpc.colorIndex = submitCubemap((int32_t)rpc.transformIndex, cube);
			}
		}

		GRAPHICS_INFO("RENDER_PIPELINE: submitted entity into pipeline");
	}

	void RenderPipeline::setContainerRenderable(MaterialRenderType materialType, ecs::RenderPipelineComponent& rpc, size_t verticesToPush, size_t indicesToPush) {
		m_containerPtr = nullptr;

		auto canPushToContainer = [verticesToPush, indicesToPush](const RenderContainer& container)->bool {
			const size_t currentVerticesSize = container.getVertices().size() + sizeof(Vertex) / 4;
			const size_t currentIndicesSize = container.getIndices().size();
			const size_t currentTransformSize = container.getTransforms().size();

			const bool cannotPushVertices = currentVerticesSize + verticesToPush >= settings::maxVerticesCount;
			const bool cannotPushIndices = currentIndicesSize + indicesToPush >= settings::maxIndicesCount;
			const bool cannotPushTransform = currentTransformSize + 1 >= 32;

			return !(cannotPushVertices || cannotPushIndices || cannotPushTransform);
		};

		auto textureContainerCheck = [&canPushToContainer, &rpc, this](std::vector<RenderContainer>& renderContainers, MaterialRenderType matType)->bool{
			for (size_t i = 0; i < renderContainers.size(); i++) {
				const bool thereIsPlaceInContainer = canPushToContainer(renderContainers[i]);

				if (thereIsPlaceInContainer) {
					m_containerPtr = &renderContainers[i];
					rpc.containerIndex = i;
					rpc.materialType = (size_t)matType;

					GRAPHICS_TRACE("RENDER_PIPELINE: available container is at index {}, returning...", i);
					return true;
				}
			}

			return false;
		};

		auto emplaceNewContainer = [&rpc, this](std::vector<RenderContainer>& containers, MaterialRenderType matType) {
			m_containerPtr = &containers.emplace_back();
			rpc.containerIndex = containers.size() - 1;
			rpc.materialType = (size_t)matType;

			GRAPHICS_INFO("RENDER_PIPELINE: emplaced back new render container at {}, current size {}", matType, containers.size());
		};

		if (materialType == MaterialRenderType::COLOR) { // we don't have "Color" containers, in order to get less draw calls
			if (textureContainerCheck(m_containers2D, MaterialRenderType::TEXTURE2D) && m_containerPtr) {
				m_containerPtr->m_materialRenderType = MaterialRenderType::TEXTURE2D;
				return; 
			}
			if (textureContainerCheck(m_containersCubemap, MaterialRenderType::CUBEMAP) && m_containerPtr) {
				m_containerPtr->m_materialRenderType = MaterialRenderType::CUBEMAP;
				return;
			}

			// if cannot find available container, create new one (for color we want to emplace new container in textures 2d)
			emplaceNewContainer(m_containers2D, MaterialRenderType::TEXTURE2D);
			m_containerPtr->m_materialRenderType = MaterialRenderType::TEXTURE2D;
			return;
		}
		else if (materialType == MaterialRenderType::TEXTURE2D) {
			if (textureContainerCheck(m_containers2D, MaterialRenderType::TEXTURE2D) && m_containerPtr) {
				m_containerPtr->m_materialRenderType = MaterialRenderType::TEXTURE2D;
				return;
			}

			// if cannot find available container, create new one
			emplaceNewContainer(m_containers2D, MaterialRenderType::TEXTURE2D);
			m_containerPtr->m_materialRenderType = MaterialRenderType::TEXTURE2D;
			return;
		}
		else if (materialType == MaterialRenderType::CUBEMAP) {
			if (textureContainerCheck(m_containersCubemap, MaterialRenderType::CUBEMAP) && m_containerPtr) {
				m_containerPtr->m_materialRenderType = MaterialRenderType::CUBEMAP;
				return;
			}

			// if cannot find available container, create new one
			emplaceNewContainer(m_containersCubemap, MaterialRenderType::CUBEMAP);
			m_containerPtr->m_materialRenderType = MaterialRenderType::CUBEMAP;
			return;
		}
	}

	void RenderPipeline::setContainerLight(ecs::RenderPipelineComponent& rpc) {
		m_lightPtr = nullptr;

		for (size_t i = 0; i < m_lights.size(); i++) {
			auto canPushToContainer = [currentLightSize = m_lights[i].m_lightMaterials.size()]()->bool {
				const bool cannotPushLights = currentLightSize + 1 >= 32;
				return !cannotPushLights;
			};

			const bool thereIsPlaceInContainer = canPushToContainer();

			if (thereIsPlaceInContainer) {
				m_lightPtr = &m_lights[i];
				rpc.containerLightIndex = i;

				GRAPHICS_TRACE("RENDER_PIPELINE: available container for light is at {}, returning...", 0);
				return;
			}
		}

		// if cannot find available container, create new one
		m_lightPtr = &m_lights.emplace_back();
		rpc.containerLightIndex = m_lights.size() - 1;

		GRAPHICS_INFO("RENDER_PIPELINE: emplaced back new render container (for light), current size {}", m_lights.size());
	}

	size_t RenderPipeline::submitRenderable(const ecs::RenderableComponent& renderable, const ecs::TransformComponent& transform) {
		if (!m_containerPtr) {
			GRAPHICS_ERROR("RENDER_PIPELINE: submitRenderable(), m_containerPtr is nullptr!");
			return -1;
		}
		
		GRAPHICS_INFO("RENDER_PIPELINE: submitting renderable component");

		auto& indicesMax = m_containerPtr->m_indicesMax;
		auto& shapeID = m_containerPtr->m_shapeID;
		auto& transforms = m_containerPtr->m_transforms;

		{ // set shapeID on every vertex for batch renderer
			auto& vertices = m_containerPtr->m_vertices;
			vertices.insert(vertices.end(), renderable.vertices.begin(), renderable.vertices.end());

			auto fromBeginOfInsertedVertices = vertices.end() - renderable.vertices.size();
			auto toItsEnd = vertices.end();
			auto modifyShaderID = [newShapeID = shapeID](Vertex& vertex) {
				vertex.shapeID = newShapeID;
			};

			std::for_each(fromBeginOfInsertedVertices, toItsEnd, modifyShaderID);
		}

		{ // increase indices for every vertex (batch renderer)
			auto& indices = m_containerPtr->m_indices;
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

		GRAPHICS_INFO("RENDER_PIPELINE: submitted renderable component {} --- vert_size = {} indi_size = {}, indicesMax = {}", renderable.name, renderable.vertices.size(), renderable.indices.size(), indicesMax);
		
		return transforms.size() - 1;
	}

	size_t RenderPipeline::submitColor(int32_t entityIndex, const ecs::ColorComponent& color) {
		if (!m_containerPtr) {
			GRAPHICS_ERROR("RENDER_PIPELINE: submitColor(), m_containerPtr is nullptr!");
			return -1;
		}

		m_containerPtr->m_colors.push_back({ entityIndex, color.texture });
		m_containerPtr->m_samplerTypes.push_back(0.0f);

		GRAPHICS_TRACE("RENDER_PIPELINE: submitted color component, current size = {}, entity_index = {}", m_containerPtr->m_colors.size(), entityIndex);
		
		return m_containerPtr->m_colors.size() - 1;
	}

	size_t RenderPipeline::submitTexture2D(int32_t entityIndex, const ecs::Texture2DComponent& texture) {
		if (!m_containerPtr) {
			GRAPHICS_ERROR("RENDER_PIPELINE: submitTexture2D(), m_containerPtr is nullptr!");
			return -1;
		}

		m_containerPtr->m_tex2D.push_back({ entityIndex, texture.texture });
		m_containerPtr->m_samplerTypes.push_back(1.0f);

		GRAPHICS_TRACE("RENDER_PIPELINE: submitted texture2d component, current size = {}, entity_index = {}, texture2D = {}", m_containerPtr->m_tex2D.size(), entityIndex, texture.texture);

		return m_containerPtr->m_tex2D.size() - 1;
	}

	size_t RenderPipeline::submitCubemap(int32_t entityIndex, const ecs::TextureCubemapComponent& cubemap) {
		if (!m_containerPtr) {
			GRAPHICS_ERROR("RENDER_PIPELINE: submitCubemap(), m_containerPtr is nullptr!");
			return -1;
		}

		m_containerPtr->m_cubes.push_back({ entityIndex, cubemap.texture });
		m_containerPtr->m_samplerTypes.push_back(2.0f);

		GRAPHICS_TRACE("RENDER_PIPELINE: submitted texture cubemap component, current size = {}, entity_index = {}, textureCubemap = {}", 
			m_containerPtr->m_cubes.size(), entityIndex, cubemap.texture);

		return m_containerPtr->m_cubes.size() - 1;
	}

	size_t RenderPipeline::submitLight(const maths::vec3& position, const ecs::LightComponent& light) {
		if (!m_containerPtr) {
			GRAPHICS_ERROR("RENDER_PIPELINE: submitLight(), m_containerPtr is nullptr!");
			return -1;
		}

		auto& lightMaterial = m_lightPtr->m_lightMaterials.emplace_back();
		lightMaterial.position = maths::vec4(position, 1.f);
		lightMaterial.ambient = light.ambient;
		lightMaterial.diffuse = light.diffuse;
		lightMaterial.specular = light.specular;
		lightMaterial.linear = light.linear;
		lightMaterial.quadratic = light.quadratic;
		lightMaterial.constant = light.constant;
		lightMaterial.shininess = light.shininess;

		GRAPHICS_TRACE("RENDER_PIPELINE: submitted light component with its center, current size = {}", m_lights.size());
		
		return m_lights.size() - 1;
	}

	void RenderPipeline::submitCamera(const RenderCamera* cam) {
		m_camera = cam;

		GRAPHICS_TRACE("RENDER_PIPELINE: submitted Camera!");
	}


}
