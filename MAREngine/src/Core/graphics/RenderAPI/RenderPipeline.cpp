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
#include "../Renderer/PipelineManager.h"
#include "RenderBufferManager.h"
#include "RenderCamera.h"
#include "../GraphicsLogs.h"
#include "../GraphicsLimits.h"
#include "../../ecs/SceneManager.h"
#include "../../ecs/Entity/Entity.h"
#include "../../ecs/Entity/EventsComponentEntity.h"


namespace marengine {


	RenderPipeline* RenderPipeline::Instance{ nullptr };


	void RenderPipeline::reset() {
		for (auto& batch : m_staticColorBatches) { batch.reset(); }
		for (auto& batch : m_staticTexture2DBatches) { batch.reset(); }

		m_staticColorBatches.clear();
		m_staticTexture2DBatches.clear();

		m_pointLightBatch.reset();

		GRAPHICS_INFO("RENDER_PIPELINE: all data was resetted!");
	}

	void RenderPipeline::pushCameraToPipeline(const RenderCamera* cam) {
		m_renderCamera = cam;

		GRAPHICS_TRACE("RENDER_PIPELINE: submitted Camera!");
	}

	void RenderPipeline::pushEntityToPipeline(const Entity& entity) {
		GRAPHICS_INFO("RENDER_PIPELINE: going to submit entity into pipeline...");

		if (entity.hasComponent<ColorComponent>()) {
			const uint32_t batchIndex{ getAvailableColorBatch(entity) };
			auto& availableBatch{ m_staticColorBatches[batchIndex] };
			availableBatch.submitToBatch(entity);

			auto& rpc{ entity.getComponent<RenderPipelineComponent>() };
			rpc.containerIndex = batchIndex;
		}
		if (entity.hasComponent<Texture2DComponent>()) {
			const uint32_t batchIndex{ getAvailableTexture2DBatch(entity) };
			auto& availableBatch{ m_staticTexture2DBatches[batchIndex] };
			availableBatch.submitToBatch(entity);

			auto& rpc{ entity.getComponent<RenderPipelineComponent>() };
			rpc.containerIndex = batchIndex;
		}
		if (entity.hasComponent<PointLightComponent>()) {
			if (m_pointLightBatch.canBeBatched(entity)) {
				m_pointLightBatch.submitEntityWithLightning(entity);
			}
		}
		if (entity.hasComponent<CameraComponent>()) {
			if (m_renderCamera) {
				const bool userCheckingGameInPlayMode{ SceneManager::Instance->isPlayMode() || SceneManager::Instance->isPauseMode() };
				const bool userModifyingGameCameraInEditorMode{ SceneManager::Instance->isEditorMode() && !SceneManager::Instance->useEditorCamera };
				if (userCheckingGameInPlayMode || userModifyingGameCameraInEditorMode) {
					pushEntityCameraToPipeline(entity);
				}
			}
			else {
				pushEntityCameraToPipeline(entity);
			}
		}

		GRAPHICS_INFO("RENDER_PIPELINE: submitted entity into pipeline");
	}

	void RenderPipeline::onBatchesReadyToDraw() {
		FPipelineManager::Instance->close();

		FRenderBufferManager::onMeshBatchReadyToDraw(m_staticColorBatches);
		FRenderBufferManager::onMeshBatchReadyToDraw(m_staticTexture2DBatches);
		FRenderBufferManager::onCreatePointLightsSSBO(m_pointLightBatch);
		FRenderBufferManager::onCreateRenderCameraSSBO(m_renderCamera);
	}

	uint32_t RenderPipeline::getAvailableColorBatch(const Entity& entity) {
		auto canBatchEntity = [&entity](FMeshBatchStaticColor& batch) {
			return batch.canBeBatched(entity);
		};
		const auto it = std::find_if(m_staticColorBatches.begin(), m_staticColorBatches.end(), canBatchEntity);
		if (it != m_staticColorBatches.end()) {
			return std::distance(m_staticColorBatches.begin(), it);
		}
		else {
			m_staticColorBatches.emplace_back();
			return m_staticColorBatches.size() - 1;
		}
	}

	uint32_t RenderPipeline::getAvailableTexture2DBatch(const Entity& entity) {
		auto canBatchEntity = [&entity](FMeshBatchStaticTexture2D& batch) {
			return batch.canBeBatched(entity);
		};
		const auto it = std::find_if(m_staticTexture2DBatches.begin(), m_staticTexture2DBatches.end(), canBatchEntity);
		if (it != m_staticTexture2DBatches.end()) {
			return std::distance(m_staticTexture2DBatches.begin(), it);
		}
		else {
			m_staticTexture2DBatches.emplace_back();
			return m_staticTexture2DBatches.size() - 1;
		}
	}

	void RenderPipeline::pushEntityCameraToPipeline(const Entity& entity) {
		auto& cameraComponent{ entity.getComponent<CameraComponent>() };
		if (cameraComponent.isMainCamera()) {
			const auto& transformComponent{ entity.getComponent<TransformComponent>() };
			RenderCamera* renderCamera{ &cameraComponent.renderCamera };
			renderCamera->calculateCameraTransforms(transformComponent, cameraComponent);
			RenderPipeline::Instance->pushCameraToPipeline(renderCamera);
		}
	}

	const std::vector<FMeshBatchStaticColor>& RenderPipeline::getColorBatches() const { 
		return m_staticColorBatches; 
	}

	const std::vector<FMeshBatchStaticTexture2D>& RenderPipeline::getTexture2DBatches() const { 
		return m_staticTexture2DBatches;
	}

	const FPointLightBatch& RenderPipeline::getPointLightBatch() const { 
		return m_pointLightBatch; 
	}

	const RenderCamera* RenderPipeline::getRenderCamera() const { 
		return m_renderCamera;
	}


}
