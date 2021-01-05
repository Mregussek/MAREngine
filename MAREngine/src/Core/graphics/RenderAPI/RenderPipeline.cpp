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


namespace marengine {


	RenderPipeline* RenderPipeline::Instance{ nullptr };


	const std::vector<FMeshBatchStaticColor>& RenderPipeline::getColorBatches() const { return m_staticColorBatches; }
	const std::vector<FMeshBatchStaticTexture2D>& RenderPipeline::getTexture2DBatches() const { return m_staticTexture2DBatches; }
	const std::vector<LightContainer>& RenderPipeline::getLightContainers() const { return m_lights; }
	const RenderCamera* RenderPipeline::getCamera() const { return m_camera; }

	void RenderPipeline::reset() {
		for (auto& batch : m_staticColorBatches) { batch.reset(); }
		for (auto& batch : m_staticTexture2DBatches) { batch.reset(); }
		for (auto& container : m_containersCubemap) { container.reset(); }
		for (auto& light : m_lights) { light.reset(); }

		m_staticColorBatches.clear();
		m_staticTexture2DBatches.clear();
		m_containersCubemap.clear();
		m_lights.clear();

		GRAPHICS_INFO("RENDER_PIPELINE: all data was resetted!");
	}

	void RenderPipeline::pushCameraToPipeline(const RenderCamera* cam) {
		m_camera = cam;

		GRAPHICS_TRACE("RENDER_PIPELINE: submitted Camera!");
	}

	void RenderPipeline::pushEntityToPipeline(const Entity& entity) {
		GRAPHICS_INFO("RENDER_PIPELINE: going to submit entity into pipeline...");

		if (entity.hasComponent<ColorComponent>()) {
			auto& availableBatch{ getAvailableColorBatch(entity) };
			availableBatch.submitToBatch(entity);
		}
		if (entity.hasComponent<Texture2DComponent>()) {
			auto& availableBatch{ getAvailableTexture2DBatch(entity) };
			availableBatch.submitToBatch(entity);
		}

		GRAPHICS_INFO("RENDER_PIPELINE: submitted entity into pipeline");
	}

	FMeshBatchStaticColor& RenderPipeline::getAvailableColorBatch(const Entity& entity) {
		auto canBatchEntity = [&entity](FMeshBatchStaticColor& batch) {
			return batch.canBeBatched(entity);
		};
		const auto it = std::find_if(m_staticColorBatches.begin(), m_staticColorBatches.end(), canBatchEntity);
		if (it != m_staticColorBatches.end()) {
			return *it;
		}
		else {
			return m_staticColorBatches.emplace_back();
		}
	}

	FMeshBatchStaticTexture2D& RenderPipeline::getAvailableTexture2DBatch(const Entity& entity) {
		auto canBatchEntity = [&entity](FMeshBatchStaticTexture2D& batch) {
			return batch.canBeBatched(entity);
		};
		const auto it = std::find_if(m_staticTexture2DBatches.begin(), m_staticTexture2DBatches.end(), canBatchEntity);
		if (it != m_staticTexture2DBatches.end()) {
			return *it;
		}
		else {
			return m_staticTexture2DBatches.emplace_back();
		}
	}

	void RenderPipeline::pushLightToPipeline(const Entity& entity) {
		const auto& tran = entity.getComponent<TransformComponent>();
		auto& rpc = entity.getComponent<RenderPipelineComponent>();
		const auto& light = entity.getComponent<LightComponent>();

		setContainerLight(rpc);
		rpc.lightIndex = submitLight(tran.center, light);
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
