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
#include "RenderPipelineHelper.h"
#include "../GraphicsLogs.h"
#include "../GraphicLimits.h"
#include "../../ecs/Entity/Entity.h"
#include "../../ecs/Entity/EntityCollection.h"


namespace marengine {


	RenderPipeline* RenderPipeline::Instance{ nullptr };


	const std::vector<RenderContainer>& RenderPipeline::getColorContainers() const { return m_containersColor; }
	const std::vector<RenderContainer>& RenderPipeline::get2Dcontainers() const { return m_containers2D; }
	const std::vector<RenderContainer>& RenderPipeline::getCubemapContainers() const { return m_containersCubemap; }
	const std::vector<LightContainer>& RenderPipeline::getLightContainers() const { return m_lights; }
	const RenderCamera* RenderPipeline::getCamera() const { return m_camera; }

	void RenderPipeline::reset() {
		for (auto& container : m_containersColor) { container.reset(); }
		for (auto& container : m_containers2D) { container.reset(); }
		for (auto& container : m_containersCubemap) { container.reset(); }
		for (auto& light : m_lights) { light.reset(); }

		m_containersColor.clear();
		m_containers2D.clear();
		m_containersCubemap.clear();
		m_lights.clear();

		GRAPHICS_INFO("RENDER_PIPELINE: all data was resetted!");
	}

	void RenderPipeline::pushCameraToPipeline(const RenderCamera* cam) {
		m_camera = cam;

		GRAPHICS_TRACE("RENDER_PIPELINE: submitted Camera!");
	}

	void RenderPipeline::pushCollectionToPipeline(const EntityCollection& collection) {
		const auto& entities{ collection.getEntities() };
		std::for_each(entities.cbegin(), entities.cend(), [this](const Entity& entity) {
			pushEntityToPipeline(entity);
		});
	}

	void RenderPipeline::pushEntityToPipeline(const Entity& entity) {
		GRAPHICS_INFO("RENDER_PIPELINE: going to submit entity into pipeline...");

		const bool hasColor = entity.hasComponent<ColorComponent>();
		const bool hasTexture2D = entity.hasComponent<Texture2DComponent>();
		const bool hasCubemap = entity.hasComponent<TextureCubemapComponent>();
		const bool hasAnyMaterial = hasColor || hasTexture2D || hasCubemap;
		const bool hasRenderable = entity.hasComponent<RenderableComponent>();

		if (hasRenderable && hasAnyMaterial) {
			const auto& tran = entity.getComponent<TransformComponent>();
			auto& renderable = entity.getComponent<RenderableComponent>();
			auto& rpc = entity.get_addComponent<RenderPipelineComponent>();

			const auto vertSize{ (uint32_t)renderable.vertices.size() };
			const auto indiSize{ (uint32_t)renderable.indices.size() };

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

	void RenderPipeline::pushLightToPipeline(const Entity& entity) {
		const auto& tran = entity.getComponent<TransformComponent>();
		auto& rpc = entity.getComponent<RenderPipelineComponent>();
		const auto& light = entity.getComponent<LightComponent>();

		setContainerLight(rpc);
		rpc.lightIndex = submitLight(tran.center, light);
	}

	void RenderPipeline::setContainerRenderable(MaterialRenderType materialType, RenderPipelineComponent& rpc, uint32_t verticesToPush, uint32_t indicesToPush) {
		m_containerPtr = nullptr;

		auto selectContainerPtrProcedure = [&rpc, materialType, verticesToPush, indicesToPush, this](std::vector<RenderContainer>& containers) {
			const auto index = RenderPipelineHelper::findAvailableRenderContainer(containers, verticesToPush, indicesToPush);
			if (index != -1) {
				GRAPHICS_TRACE("RENDER_PIPELINE: found render container with place for new renderable, index {}, materialType {}", index, materialType);
				m_containerPtr = &containers[index];
				rpc.containerIndex = index;
			}
			else {
				GRAPHICS_TRACE("RENDER_PIPELINE: cannot find render container with place in it for materialType {}, creating new one...", index, materialType);
				m_containerPtr = &containers.emplace_back();
				rpc.containerIndex = containers.size() - 1;
			}

			rpc.materialType = (size_t)materialType;
			m_containerPtr->m_materialRenderType = materialType;
		};

		if (materialType == MaterialRenderType::COLOR) {
			selectContainerPtrProcedure(m_containersColor);
		}
		else if (materialType == MaterialRenderType::TEXTURE2D) {
			selectContainerPtrProcedure(m_containers2D);
		}
		else if (materialType == MaterialRenderType::CUBEMAP) {
			selectContainerPtrProcedure(m_containersCubemap);
		}
	}

	void RenderPipeline::setContainerLight(RenderPipelineComponent& rpc) {
		m_lightPtr = nullptr;

		const auto index = RenderPipelineHelper::findAvailableLightContainer(m_lights);
		if (index != -1) {
			m_lightPtr = &m_lights[index];
			rpc.containerLightIndex = index;
		}
		else {
			m_lightPtr = &m_lights.emplace_back();
			rpc.containerLightIndex = m_lights.size() - 1;
		}
	}

	size_t RenderPipeline::submitRenderable(const RenderableComponent& renderable, const TransformComponent& transform) {
		if (!m_containerPtr) {
			GRAPHICS_ERROR("RENDER_PIPELINE: submitRenderable(), m_containerPtr is nullptr!");
			return -1;
		}
		GRAPHICS_TRACE("RENDER_PIPELINE: submitting renderable component...");

		auto& indicesMax = m_containerPtr->m_indicesMax;
		auto& shapeID = m_containerPtr->m_shapeID;

		submitVertices(renderable.vertices, shapeID);
		submitIndices(renderable.indices, indicesMax);
		submitTransform(transform.getTransform());

		indicesMax += (renderable.vertices.size() * sizeof(Vertex) / 4) / RenderContainer::getStride();
		shapeID++;

		MAR_CORE_ASSERT(m_containerPtr->m_transforms.size() == shapeID, "transform.size() and shapeID are not equal!");
		return m_containerPtr->m_transforms.size() - 1;
	}

	void RenderPipeline::submitVertices(const std::vector<Vertex>& vertices, float shapeID) {
		GRAPHICS_TRACE("RENDER_PIPELINE: submitting vertices and setting shapeID for batch renderer...");

		auto& containerVertices{ m_containerPtr->m_vertices };
		containerVertices.insert(containerVertices.end(), vertices.begin(), vertices.end());

		auto fromBeginOfInsertedVertices = containerVertices.end() - vertices.size();
		auto toItsEnd = containerVertices.end();
		auto modifyShaderID = [shapeID](Vertex& vertex) {
			vertex.shapeID = shapeID;
		};

		std::for_each(fromBeginOfInsertedVertices, toItsEnd, modifyShaderID);
	}

	void RenderPipeline::submitIndices(const std::vector<uint32_t>& indices, uint32_t indicesMax) {
		GRAPHICS_TRACE("RENDER_PIPELINE: submitting indices and increasing every indice for batch renderer...");

		auto& containerIndices{ m_containerPtr->m_indices };
		containerIndices.insert(containerIndices.end(), indices.begin(), indices.end());

		auto fromBeginOfInsertedIndices = containerIndices.end() - indices.size();
		auto toItsEnd = containerIndices.end();
		auto extendIndices = [extension = indicesMax](uint32_t& indice) {
			indice += extension;
		};

		std::for_each(fromBeginOfInsertedIndices, toItsEnd, extendIndices);
	}

	void RenderPipeline::submitTransform(const maths::mat4& transform) {
		GRAPHICS_TRACE("RENDER_PIPELINE: submitting transform...");

		m_containerPtr->m_transforms.push_back(transform);
	}

	size_t RenderPipeline::submitColor(int32_t entityIndex, const ColorComponent& color) {
		GRAPHICS_TRACE("RENDER_PIPELINE: submitting color component, current size = {}, entity_index = {}", m_containerPtr->m_colors.size(), entityIndex);

		m_containerPtr->m_colors.push_back({ entityIndex, color.texture });
		m_containerPtr->m_samplerTypes.push_back(0.0f);
		return m_containerPtr->m_colors.size() - 1;
	}

	size_t RenderPipeline::submitTexture2D(int32_t entityIndex, const Texture2DComponent& texture) {
		GRAPHICS_TRACE("RENDER_PIPELINE: submitting texture2d component, current size = {}, entity_index = {}, texture2D = {}", m_containerPtr->m_tex2D.size(), entityIndex, texture.texture);

		m_containerPtr->m_tex2D.push_back({ entityIndex, texture.texture });
		m_containerPtr->m_samplerTypes.push_back(1.0f);
		return m_containerPtr->m_tex2D.size() - 1;
	}

	size_t RenderPipeline::submitCubemap(int32_t entityIndex, const TextureCubemapComponent& cubemap) {
		GRAPHICS_TRACE("RENDER_PIPELINE: submitting texture cubemap component, current size = {}, entity_index = {}, textureCubemap = {}", m_containerPtr->m_cubes.size(), entityIndex, cubemap.texture);

		m_containerPtr->m_cubes.push_back({ entityIndex, cubemap.texture });
		m_containerPtr->m_samplerTypes.push_back(2.0f);
		return m_containerPtr->m_cubes.size() - 1;
	}

	size_t RenderPipeline::submitLight(const maths::vec3& position, const LightComponent& light) {
		if (!m_lightPtr) {
			GRAPHICS_ERROR("RENDER_PIPELINE: submitLight(), m_lightPtr is nullptr!");
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

		GRAPHICS_TRACE("RENDER_PIPELINE: submitted light component with its center, current size = {}", m_lightPtr->m_lightMaterials.size());
		
		return m_lightPtr->m_lightMaterials.size() - 1;
	}


}
