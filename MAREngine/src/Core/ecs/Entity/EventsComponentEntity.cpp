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


#include "EventsComponentEntity.h"
#include "Entity.h"
#include "../SceneManager.h"
#include "../Scene.h"
#include "../Components/Components.h"
#include "../../graphics/Mesh/EventsMeshBatchStatic.h"
#include "../../graphics/Lightning/EventsLightBatch.h"
#include "../../graphics/RenderAPI/RenderPipeline.h"
#include "../../graphics/RenderAPI/RenderBufferManager.h"


namespace marengine {


	const FEventsComponentEntity* FEventsComponentEntity::Instance{ nullptr };
	const FEventsCameraEntity* FEventsCameraEntity::Instance{ nullptr };


	void FEventsCameraEntity::onMainCameraUpdate(const Entity& entity) const {
		auto& cameraComponent{ entity.getComponent<CameraComponent>() };

		auto updateCameraOperation = [&entity, &cameraComponent, this]() {
			const auto& transform = entity.getComponent<TransformComponent>();
			cameraComponent.renderCamera.calculateCameraTransforms(transform, cameraComponent);
			FRenderBufferManager::onRenderCameraUpdate(&cameraComponent.renderCamera);
		};

		const bool userCheckingGameInPlayMode{
			SceneManager::Instance->isPlayMode() || SceneManager::Instance->isPauseMode()
		};

		const bool userModifyingGameCameraInEditorMode{
			SceneManager::Instance->isEditorMode() && !SceneManager::Instance->useEditorCamera
		};

		if (userCheckingGameInPlayMode || userModifyingGameCameraInEditorMode) {
			updateCameraOperation();
		}
	}

	void FEventsCameraEntity::onEditorCameraSet(const RenderCamera* renderCamera) const {
		RenderPipeline::Instance->pushCameraToPipeline(renderCamera);
		FRenderBufferManager::onRenderCameraUpdate(renderCamera);
	}

	void FEventsCameraEntity::onGameCameraSet() const {
		Scene* scene{ SceneManager::Instance->getScene() };

		auto hasMainCamera = [&scene](entt::entity entity) {
			const auto& cam = scene->getComponent<CameraComponent>(entity);
			return cam.isMainCamera();
		};

		auto view = scene->getView<CameraComponent>();
		view.each([&scene](entt::entity entt_entity, CameraComponent& cameraComponent) {
			if (cameraComponent.isMainCamera()) {
				const auto& transform{ scene->getComponent<TransformComponent>(entt_entity) };
				
				cameraComponent.renderCamera.calculateCameraTransforms(transform, cameraComponent);

				RenderPipeline::Instance->pushCameraToPipeline(&cameraComponent.renderCamera);
				FRenderBufferManager::onRenderCameraUpdate(&cameraComponent.renderCamera);
			}
		});
	}

	/**************************************************************************************************
	***************************** TRANSFORM COMPONENT TEMPLATES ***************************************
	***************************************************************************************************/

	template<> void FEventsComponentEntity::onUpdate<TransformComponent>(const Entity& entity) const {
		const auto& transform = entity.getComponent<TransformComponent>();
		const auto& renderPipelineComponent{ entity.getComponent<RenderPipelineComponent>() };

		if (renderPipelineComponent.materialType > 0) { // need to check if it is rendered
			FEventsMeshBatchStatic::onTransformUpdate(entity);
		}

		if (entity.hasComponent<CameraComponent>()) {
			const auto& cameraComponent{ entity.getComponent<CameraComponent>() };
			if (cameraComponent.isMainCamera()) {
				FRenderBufferManager::onRenderCameraUpdate(&cameraComponent.renderCamera);
			}
		}

		if (entity.hasComponent<LightComponent>()) {
			FEventsLightBatch::onPointLightPositionUpdate(entity);
		}
	}

	/**************************************************************************************************
	***************************** RENDERABLE COMPONENT TEMPLATES ***************************************
	***************************************************************************************************/

	template<> void FEventsComponentEntity::onAdd<RenderableComponent>(const Entity& entity) const {
		entity.addComponent<RenderableComponent>();
		SceneManager::Instance->initialize();
	}

	template<> void FEventsComponentEntity::onUpdate<RenderableComponent>(const Entity& entity) const {
		SceneManager::Instance->initialize();
	}

	template<> void FEventsComponentEntity::onRemove<RenderableComponent>(const Entity& entity) const {
		entity.removeComponent<RenderableComponent>();
		SceneManager::Instance->initialize();
	}

	/**************************************************************************************************
	***************************** COLOR COMPONENT TEMPLATES ***************************************
	***************************************************************************************************/

	template<> void FEventsComponentEntity::onAdd<ColorComponent>(const Entity& entity) const {
		entity.addComponent<ColorComponent>();
		SceneManager::Instance->initialize();
	}

	template<> void FEventsComponentEntity::onUpdate<ColorComponent>(const Entity& entity) const {
		FEventsMeshBatchStatic::onColorUpdate(entity);
	}

	template<> void FEventsComponentEntity::onRemove<ColorComponent>(const Entity& entity) const {
		entity.removeComponent<ColorComponent>();
		SceneManager::Instance->initialize();
	}

	/**************************************************************************************************
	***************************** TEXTURE 2D COMPONENT TEMPLATES **************************************
	***************************************************************************************************/

	template<> void FEventsComponentEntity::onUpdate<Texture2DComponent>(const Entity& entity) const {
		SceneManager::Instance->initialize();
	}

	template<> void FEventsComponentEntity::onRemove<Texture2DComponent>(const Entity& entity) const {
		entity.removeComponent<Texture2DComponent>();
		SceneManager::Instance->initialize();
	}

	/**************************************************************************************************
	***************************** TEXTURE CUBEMAP COMPONENT TEMPLATES *********************************
	***************************************************************************************************/

	template<> void FEventsComponentEntity::onUpdate<TextureCubemapComponent>(const Entity& entity) const {
		SceneManager::Instance->initialize();
	}

	template<> void FEventsComponentEntity::onRemove<TextureCubemapComponent>(const Entity& entity) const {
		entity.removeComponent<TextureCubemapComponent>();
		SceneManager::Instance->initialize();
	}

	/**************************************************************************************************
	***************************** LIGHT COMPONENT TEMPLATES ***************************************
	***************************************************************************************************/

	template<> void FEventsComponentEntity::onAdd<LightComponent>(const Entity& entity) const {
		entity.addComponent<LightComponent>();
		SceneManager::Instance->initialize();
	}

	template<> void FEventsComponentEntity::onUpdate<LightComponent>(const Entity& entity) const {
		FEventsLightBatch::onPointLightUpdate(entity);
	}

	template<> void FEventsComponentEntity::onRemove<LightComponent>(const Entity& entity) const {
		entity.removeComponent<LightComponent>();
		SceneManager::Instance->initialize();
	}

	/**************************************************************************************************
	***************************** CAMERA COMPONENT TEMPLATES ***************************************
	***************************************************************************************************/

	template<> void FEventsComponentEntity::onRemove<CameraComponent>(const Entity& entity) const {
		const auto& cameraComponent{ entity.getComponent<CameraComponent>() };
		if (cameraComponent.isMainCamera()) {
			// cannot remove main camera!
		}
		else {
			entity.removeComponent<CameraComponent>();
		}
	}


}
