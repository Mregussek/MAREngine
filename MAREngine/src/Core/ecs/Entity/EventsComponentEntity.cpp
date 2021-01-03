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
#include "../ECSLogs.h"
#include "../../graphics/RenderAPI/RenderManager.h"
#include "../../graphics/RenderAPI/RenderManagerEvents.h"
#include "../SceneManager.h"
#include "../Scene.h"


namespace marengine {


	const FEventsComponentEntity* FEventsComponentEntity::Instance{ nullptr };


	void FEventsComponentEntity::onMainCameraUpdate(const Entity& entity) const {
		ECS_DEBUG("F_EVENTS_COMPONENT_ENTITY: onMainCameraUpdate, entity {}", entity.getComponent<TagComponent>().tag);

		auto& cameraComponent{ entity.getComponent<CameraComponent>() };

		const bool userCheckingGameInPlayMode{
			SceneManager::Instance->isPlayMode() || SceneManager::Instance->isPauseMode()
		};

		const bool userModifyingGameCameraInEditorMode{
			SceneManager::Instance->isEditorMode() && !SceneManager::Instance->useEditorCamera
		};

		if (userCheckingGameInPlayMode || userModifyingGameCameraInEditorMode) {
			ECS_TRACE("F_EVENTS_COMPONENT_ENTITY: updating in-game camera, because user is using it...");

			const auto& transform = entity.getComponent<TransformComponent>();
			cameraComponent.renderCamera.calculateCameraTransforms(transform, cameraComponent);
			FRenderManagerEvents::onRenderCameraUpdate(&cameraComponent.renderCamera);
		}
		else {
			ECS_TRACE("F_EVENTS_COMPONENT_ENTITY: user is using editor camera, so in-game camera won't be updated!");
		}
	}

	void FEventsComponentEntity::onEditorCameraSet(const RenderCamera* camera) const {
		ECS_DEBUG("F_EVENTS_COMPONENT_ENTITY: onEditorCameraSet");

		FRenderManager::Instance->setRenderCamera(camera);
	}

	void FEventsComponentEntity::onGameCameraSet() const {
		ECS_DEBUG("F_EVENTS_COMPONENT_ENTITY: onGameCameraSet");

		Scene* scene{ SceneManager::Instance->getScene() };

		auto view = scene->getView<CameraComponent>();
		view.each([&scene](entt::entity entt_entity, CameraComponent& cameraComponent) {
			if (cameraComponent.isMainCamera()) {
				ECS_DEBUG("F_EVENTS_COMPONENT_ENTITY: onGameCameraSet, found main in-game cameraComponent, setting...");

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
		ECS_DEBUG("F_EVENTS_COMPONENT_ENTITY: updating transform component of entity {}", entity.getComponent<TagComponent>().tag);

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
		ECS_DEBUG("F_EVENTS_COMPONENT_ENTITY: adding renderable component to entity {}...", entity.getComponent<TagComponent>().tag);

		entity.addComponent<RenderableComponent>();
	}

	template<> void FEventsComponentEntity::onUpdate<RenderableComponent>(const Entity& entity) const {
		ECS_DEBUG("F_EVENTS_COMPONENT_ENTITY: updated renderable component at entity {}, current renderable - {}"
			, entity.getComponent<TagComponent>().tag, entity.getComponent<RenderableComponent>().name);

		SceneManager::Instance->initialize();
	}

	template<> void FEventsComponentEntity::onRemove<RenderableComponent>(const Entity& entity) const {
		ECS_DEBUG("F_EVENTS_COMPONENT_ENTITY: removing renderable component from entity {}...", entity.getComponent<TagComponent>().tag);

		entity.removeComponent<RenderableComponent>();
		SceneManager::Instance->initialize();
	}

	/**************************************************************************************************
	***************************** COLOR COMPONENT TEMPLATES ***************************************
	***************************************************************************************************/

	template<> void FEventsComponentEntity::onAdd<ColorComponent>(const Entity& entity) const {
		ECS_DEBUG("F_EVENTS_COMPONENT_ENTITY: adding color component to entity {}...", entity.getComponent<TagComponent>().tag);

		entity.addComponent<ColorComponent>();
		SceneManager::Instance->initialize();
	}

	template<> void FEventsComponentEntity::onUpdate<ColorComponent>(const Entity& entity) const {
		ECS_DEBUG("F_EVENTS_COMPONENT_ENTITY: updated color component at entity {}", entity.getComponent<TagComponent>().tag);

		FRenderManagerEvents::onColorAtMeshUpdate(entity);
	}

	template<> void FEventsComponentEntity::onRemove<ColorComponent>(const Entity& entity) const {
		ECS_DEBUG("F_EVENTS_COMPONENT_ENTITY: removing color component from entity {}...", entity.getComponent<TagComponent>().tag);

		entity.removeComponent<ColorComponent>();
		SceneManager::Instance->initialize();
	}

	/**************************************************************************************************
	***************************** TEXTURE 2D COMPONENT TEMPLATES **************************************
	***************************************************************************************************/

	template<> void FEventsComponentEntity::onAdd<Texture2DComponent>(const Entity& entity) const {
		ECS_DEBUG("F_EVENTS_COMPONENT_ENTITY: adding Texture2DComponent component to entity {}...", entity.getComponent<TagComponent>().tag);

		entity.addComponent<Texture2DComponent>();
		SceneManager::Instance->initialize();
	}

	template<> void FEventsComponentEntity::onUpdate<Texture2DComponent>(const Entity& entity) const {
		ECS_DEBUG("F_EVENTS_COMPONENT_ENTITY: updated Texture2DComponent component at entity {}", entity.getComponent<TagComponent>().tag);

		FRenderManagerEvents::onTexture2DatMeshUpdate(entity);
	}

	template<> void FEventsComponentEntity::onRemove<Texture2DComponent>(const Entity& entity) const {
		ECS_DEBUG("F_EVENTS_COMPONENT_ENTITY: removing Texture2DComponent component from entity {}...", entity.getComponent<TagComponent>().tag);

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
		ECS_DEBUG("F_EVENTS_COMPONENT_ENTITY: adding LightComponent component to entity {}...", entity.getComponent<TagComponent>().tag);

		entity.addComponent<LightComponent>();
		SceneManager::Instance->initialize();
	}

	template<> void FEventsComponentEntity::onUpdate<LightComponent>(const Entity& entity) const {
		ECS_DEBUG("F_EVENTS_COMPONENT_ENTITY: updated LightComponent component at entity {}...", entity.getComponent<TagComponent>().tag);

		FRenderManagerEvents::onPointLightAtBatchUpdate(entity);
	}

	template<> void FEventsComponentEntity::onRemove<LightComponent>(const Entity& entity) const {
		ECS_DEBUG("F_EVENTS_COMPONENT_ENTITY: removing LightComponent component from entity {}...", entity.getComponent<TagComponent>().tag);

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
