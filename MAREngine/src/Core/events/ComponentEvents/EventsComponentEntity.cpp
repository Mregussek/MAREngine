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
#include "../../graphics/RenderAPI/RenderManager.h"
#include "../../graphics/RenderAPI/RenderManagerEvents.h"
#include "../../ecs/SceneManager.h"
#include "../../ecs/Scene.h"


namespace marengine {


	const FEventsComponentEntity* FEventsComponentEntity::Instance{ nullptr };


	void FEventsComponentEntity::onMainCameraUpdate(const Entity& entity) const {
		auto& cameraComponent{ entity.getComponent<CameraComponent>() };

		const bool userCheckingGameInPlayMode{
			SceneManager::Instance->isPlayMode() || SceneManager::Instance->isPauseMode()
		};

		const bool userModifyingGameCameraInEditorMode{
			SceneManager::Instance->isEditorMode() && !SceneManager::Instance->useEditorCamera
		};

		if (userCheckingGameInPlayMode || userModifyingGameCameraInEditorMode) {
			const auto& transform = entity.getComponent<TransformComponent>();
			cameraComponent.renderCamera.calculateCameraTransforms(transform, cameraComponent);
			FRenderManagerEvents::onRenderCameraUpdate(&cameraComponent.renderCamera);
		}
	}

	void FEventsComponentEntity::onEditorCameraSet(const RenderCamera* camera) const {
		FRenderManager::Instance->setRenderCamera(camera);
	}

	void FEventsComponentEntity::onGameCameraSet() const {
		Scene* scene{ SceneManager::Instance->getScene() };

		auto view = scene->getView<CameraComponent>();
		view.each([&scene](entt::entity entt_entity, CameraComponent& cameraComponent) {
			if (cameraComponent.isMainCamera()) {
				const auto& transform{ scene->getComponent<TransformComponent>(entt_entity) };
				
				cameraComponent.renderCamera.calculateCameraTransforms(transform, cameraComponent);

				FRenderManager::Instance->setRenderCamera(&cameraComponent.renderCamera);
			}
		});
	}

	/**************************************************************************************************
	***************************** TRANSFORM COMPONENT TEMPLATES ***************************************
	***************************************************************************************************/

	template<> void FEventsComponentEntity::onUpdate<TransformComponent>(const Entity& entity) const {
		if (entity.hasComponent<CameraComponent>()) {
			const auto& camera{ entity.getComponent<CameraComponent>() };
			if (camera.isMainCamera()) { 
				onMainCameraUpdate(entity); 
			}
		}

		if (entity.hasComponent<LightComponent>()) {
			FRenderManagerEvents::onPointLightAtBatchUpdate(entity);
		}

		if (entity.hasComponent<RenderableComponent>() && entity.hasAnyMaterial()) {
			FRenderManagerEvents::onTransformAtMeshUpdate(entity);
		}
	}

	/**************************************************************************************************
	***************************** RENDERABLE COMPONENT TEMPLATES ***************************************
	***************************************************************************************************/

	template<> void FEventsComponentEntity::onAdd<RenderableComponent>(const Entity& entity) const {
		entity.addComponent<RenderableComponent>();
	}

	template<> void FEventsComponentEntity::onUpdate<RenderableComponent>(const Entity& entity) const {
		SceneManager::Instance->initialize();
		const auto submitReturnInfo{ FRenderManager::Instance->submitEntityRenderableToBatch(entity) };
		if (submitReturnInfo.submitted) {
			FRenderManagerEvents::onVertexIndexBuffersUpdate(*submitReturnInfo.pMeshBatch);
		}
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
		const auto submitReturnInfo{ FRenderManager::Instance->submitEntityRenderableToBatch(entity) };
		if (submitReturnInfo.submitted) {
			FRenderManagerEvents::onVertexIndexBuffersUpdate(*submitReturnInfo.pMeshBatch);
		}
	}

	template<> void FEventsComponentEntity::onUpdate<ColorComponent>(const Entity& entity) const {
		FRenderManagerEvents::onColorAtMeshUpdate(entity);
	}

	template<> void FEventsComponentEntity::onRemove<ColorComponent>(const Entity& entity) const {
		entity.removeComponent<ColorComponent>();
		SceneManager::Instance->initialize();
	}

	/**************************************************************************************************
	***************************** TEXTURE 2D COMPONENT TEMPLATES **************************************
	***************************************************************************************************/

	template<> void FEventsComponentEntity::onAdd<Texture2DComponent>(const Entity& entity) const {
		entity.addComponent<Texture2DComponent>();
		const auto submitReturnInfo{ FRenderManager::Instance->submitEntityRenderableToBatch(entity) };
		if (submitReturnInfo.submitted) {
			FRenderManagerEvents::onVertexIndexBuffersUpdate(*submitReturnInfo.pMeshBatch);
		}
	}

	template<> void FEventsComponentEntity::onUpdate<Texture2DComponent>(const Entity& entity) const {
		FRenderManagerEvents::onTexture2DatMeshUpdate(entity);
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
		FRenderManager::Instance->submitEntityLightToBatch(entity);
	}

	template<> void FEventsComponentEntity::onUpdate<LightComponent>(const Entity& entity) const {
		FRenderManagerEvents::onPointLightAtBatchUpdate(entity);
	}

	template<> void FEventsComponentEntity::onRemove<LightComponent>(const Entity& entity) const {
		entity.removeComponent<LightComponent>();
		SceneManager::Instance->initialize();
	}

	/**************************************************************************************************
	***************************** CAMERA COMPONENT TEMPLATES ***************************************
	***************************************************************************************************/

	template<> void FEventsComponentEntity::onUpdate<CameraComponent>(const Entity& entity) const {
		const auto& cameraComponent{ entity.getComponent<CameraComponent>() };
		if (cameraComponent.isMainCamera()) {
			onMainCameraUpdate(entity);
		}
	}

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
