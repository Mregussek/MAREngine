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


#include "SceneEvents.h"
#include "SceneManager.h"
#include "Entity/Entity.h"
#include "Entity/EntityCollection.h"
#include "ECSLogs.h"
#include "../graphics/Renderer/RenderPipeline.h"
#include "../graphics/Renderer/RenderEvents.h"
#include "../graphics/Renderer/RenderCamera.h"


namespace mar::ecs {
	

	SceneEvents SceneEvents::s_instance;


	SceneEvents& SceneEvents::Instance() {
		return s_instance; 
	}

	void SceneEvents::setSceneManager(SceneManager& manager) {
		m_sceneManager = &manager; 
	}

	void SceneEvents::onTransformUpdate(const Entity& entity) const {
		const auto& transform = entity.getComponent<TransformComponent>();
		const auto& rpc = entity.getComponent<RenderPipelineComponent>();

		graphics::RenderEvents::Instance().onTransformMat4Update(transform, rpc);

		if (entity.hasComponent<CameraComponent>()) {
			const auto& camera{ entity.getComponent<CameraComponent>() };
			if (camera.checkIfMain()) { onMainCameraUpdate(entity); }
		}

		if (entity.hasComponent<LightComponent>()) {
			graphics::RenderEvents::Instance().onLightPositionUpdate(transform.center, rpc);
		}

		ECS_TRACE("SCENE_EVENTS: updateTransform!");
	}

	void SceneEvents::onRenderableAdd() const {
		m_sceneManager->initialize();
	}

	void SceneEvents::onRenderableUpdate(const Entity* e) const {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: updateRenderables!");
	}
	
	void SceneEvents::onRenderableRemove() const {
		m_sceneManager->initialize();
	}

	void SceneEvents::onCameraAdd() const {
		m_sceneManager->initialize();
	}

	void SceneEvents::onMainCameraUpdate(const Entity& entity) const {
		const auto& camera = entity.getComponent<CameraComponent>();

		auto updateCameraOperation = [&entity, &camera, this]() {
			const auto& transform = entity.getComponent<TransformComponent>();
			auto& renderCamera = m_sceneManager->getScene()->getRenderCamera();
			renderCamera.calculateCameraTransforms(transform, camera);
		};

		if (m_sceneManager->isPlayMode() || m_sceneManager->isPauseMode()) { updateCameraOperation(); }
		else if (m_sceneManager->isEditorMode() && !m_sceneManager->useEditorCamera) {
			updateCameraOperation();
		}

		ECS_TRACE("SCENE_EVENTS: updatedCamera!");
	}

	void SceneEvents::onCameraRemove() const {
		m_sceneManager->initialize();
	}

	void SceneEvents::onColorAdd() const {
		m_sceneManager->initialize();
	}

	void SceneEvents::onColorUpdate(const Entity& e) const {
		const auto& color = e.getComponent<ColorComponent>();
		const auto& rpc = e.getComponent<RenderPipelineComponent>();

		graphics::RenderEvents::Instance().onColorUpdate(color.texture, rpc);

		ECS_TRACE("SCENE_EVENTS: updatedColor!");
	}

	void SceneEvents::onColorRemove() const {
		m_sceneManager->initialize();
	}

	void SceneEvents::onTexture2DAdd() const {
		m_sceneManager->initialize();
	}

	void SceneEvents::onTexture2DUpdate(const Entity* e) const {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: updatedTexture2D!");
	}

	void SceneEvents::onTexture2DRemove() const {
		m_sceneManager->initialize();
	}

	void SceneEvents::onTextureCubemapAdd() const {
		m_sceneManager->initialize();
	}

	void SceneEvents::onTextureCubemapUpdate(const Entity* e) const {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: updatedCubemap!");
	}

	void SceneEvents::onTextureCubemapRemove() const {
		m_sceneManager->initialize();
	}

	void SceneEvents::onLightAdd() const {
		m_sceneManager->initialize();
	}

	void SceneEvents::onLightUpdate(const Entity& e) const {
		const auto& transform = e.getComponent<TransformComponent>();
		const auto& light = e.getComponent<LightComponent>();
		const auto& rpc = e.getComponent<RenderPipelineComponent>();

		graphics::RenderEvents::Instance().onLightUpdate(transform.center, light, rpc);

		ECS_TRACE("SCENE_EVENTS: updatedLight!");
	}

	void SceneEvents::onLightRemove() const {
		m_sceneManager->initialize();
	}

	void SceneEvents::onScriptAdd() const {
		m_sceneManager->initialize();
	}

	void SceneEvents::onScriptUpdate(const Entity* e) const {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: updatedScript!");
	}

	void SceneEvents::onScriptRemove() const {
		m_sceneManager->initialize();
	}

	void SceneEvents::onEditorCameraSet(const graphics::RenderCamera* camera) const {
		graphics::RenderPipeline::Instance().submitCamera(camera);
	}

	void SceneEvents::onGameCameraSet() const {
		const auto& scene = m_sceneManager->getScene();

		auto hasMainCamera = [&scene](entt::entity entity) {
			const auto& cam = scene->getComponent<CameraComponent>(entity);
			return cam.checkIfMain();
		};

		auto view = scene->getView<CameraComponent>();

		const auto itEntity = std::find_if(view.begin(), view.end(), hasMainCamera);
		if (itEntity != view.end()) {
			const auto& cam = scene->getComponent<CameraComponent>(*itEntity);
			const auto& transform = scene->getComponent<TransformComponent>(*itEntity);
			auto& renderCamera = scene->getRenderCamera();
			renderCamera.calculateCameraTransforms(transform, cam);

			graphics::RenderPipeline::Instance().submitCamera(&renderCamera);
		}
	}

	void SceneEvents::onEntityCopy(const Entity& entity) const {
		graphics::RenderPipeline::Instance().submitEntity(entity);

		ECS_TRACE("SCENE_EVENTS: onEntityCopy");
	}

	void SceneEvents::onEntityRemove() const {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: onEntityRemove");
	}

	void SceneEvents::onCollectionTransformUpdate(const EntityCollection* collection, const TransformComponent& transform) const {
		auto createRelativeTransformToCollection = [&transform, this](const Entity& entity) {
			auto& entityTransform = entity.getComponent<TransformComponent>();

			entityTransform.center += transform.center;
			entityTransform.angles += transform.angles;
			entityTransform.scale += transform.scale;

			onTransformUpdate(entity);
		};

		const auto& entities = collection->getEntities();
		std::for_each(entities.cbegin(), entities.cend(), createRelativeTransformToCollection);

		ECS_TRACE("SCENE_EVENTS: onCollectionTransformUpdate");
	}

	void SceneEvents::onCollectionTransformReset(const EntityCollection* collection) const {
		auto resetToDefaultPosition = [&transform = collection->getComponent<TransformComponent>(), this](const Entity& entity) {
			entity.replaceComponent<TransformComponent>(transform);
			onTransformUpdate(entity);
		};

		const auto& entities = collection->getEntities();
		std::for_each(entities.cbegin(), entities.cend(), resetToDefaultPosition);
	}

	void SceneEvents::onCollectionCopy(const EntityCollection& collection) const {
		const auto& entities = collection.getEntities();
		std::for_each(entities.cbegin(), entities.cend(), [this](const Entity& entity) {
			onEntityCopy(entity);
		});

		ECS_TRACE("SCENE_EVENTS: onCollectionCopy");
	}

	void SceneEvents::onCollectionRemove() const {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: onCollectionRemove");
	}

	void SceneEvents::onCollectionOBJloaded(const EntityCollection& collection) const {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: onCollectionOBJloaded");
	}


}
