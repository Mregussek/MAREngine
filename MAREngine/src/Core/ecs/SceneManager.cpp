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


#include "SceneManager.h"

#include "ECSLogs.h"
#include "SceneEvents.h"

#include "../../Editor/Camera/Camera.h"

#include "../graphics/Renderer/RenderCamera.h"
#include "../graphics/Renderer/RenderPipeline.h"


namespace mar::ecs {


	void SceneManager::initialize() const {
		ECS_TRACE("SCENE_MANAGER: going to initialize!");

		auto& render_pipeline = RenderPipeline::getInstance();
		render_pipeline.reset();

		const auto& entitiesVector = m_scene->getEntities();
		const auto& collectionsVector = m_scene->getCollections();

		auto pushEntityToPipeline = [&render_pipeline](const Entity& entity) {
			render_pipeline.submitEntity(entity);
		};

		auto pushCollectionToPipeline = [&render_pipeline, &pushEntityToPipeline](const EntityCollection& collection) {
			const auto& entitiesVector = collection.getEntities();
			std::for_each(entitiesVector.cbegin(), entitiesVector.cend(), pushEntityToPipeline);
		};

		std::for_each(entitiesVector.cbegin(), entitiesVector.cend(), pushEntityToPipeline);
		std::for_each(collectionsVector.cbegin(), collectionsVector.cend(), pushCollectionToPipeline);

		ECS_INFO("SCENE_MANAGER: initialized!");
	}

	void SceneManager::shutdown() { 
		ECS_TRACE("SCENE_MANAGER: going to shutdown scene manager");

		m_scene->shutdown(); 
		delete m_scene;

		ECS_INFO("SCENE_MANAGER: called shutdown method");
	}

	void SceneManager::update() {
		ECS_TRACE("SCENE_MANAGER: going to update");

		if (m_EditorMode) { updateEditorMode(); }
		else {
			if (!m_PauseMode) { updatePlayMode(); }
			else { updatePauseMode(); }	
		}

		ECS_INFO("SCENE_MANAGER: updated!");
	}

	// -------------------------------------------------------------
	// EDITOR MODE
	// -------------------------------------------------------------

	void SceneManager::updateEditorMode() {
		ECS_TRACE("SCENE_MANAGER: updating editor mode");

		if (useEditorCamera) {
			auto camdata = editor::Camera::getCameraData();
			auto& cam = m_scene->getRenderCamera();

			cam = camdata;

			ECS_TRACE("SCENE: initializing editor camera on scene");
		}
		else {
			for (auto& entity : m_scene->getEntities()) {
				submitCameraIfPossible(entity);
			}
		}

		graphics::RenderPipeline::getInstance().submitCamera(&m_scene->getRenderCamera());

		ECS_INFO("SCENE_MANAGER: updated editor mode!");
	}

	// -------------------------------------------------------------
	// PLAY MODE
	// -------------------------------------------------------------

	void SceneManager::initPlayMode() {
		ECS_TRACE("SCENE_MANAGER: going to initialize play mode");

		m_playStorage.clear();

		const auto& entitiesVector = m_scene->getEntities();
		const auto& collectionsVector = m_scene->getCollections();

		auto inititializeScriptModule = [](const Entity& entity) {
			if (entity.hasComponent<ScriptComponent>()) {
				auto& script = entity.getComponent<ScriptComponent>();
				script.pythonScript.loadScript(script.script);
				script.pythonScript.start(entity);
			}
		};

		std::for_each(entitiesVector.cbegin(), entitiesVector.cend(), [&playStorage = m_playStorage, &inititializeScriptModule](const Entity& entity) {
			playStorage.pushEntityToStorage(entity);
			inititializeScriptModule(entity);
		});

		std::for_each(collectionsVector.cbegin(), collectionsVector.cend(), [&playStorage = m_playStorage, &inititializeScriptModule](const EntityCollection& collection) {
			playStorage.pushCollectionToStorage(collection);
			const auto& entitiesVector = collection.getEntities();
			std::for_each(entitiesVector.cbegin(), entitiesVector.cend(), inititializeScriptModule);
		});

		ECS_INFO("SCENE_MANAGER: initialized play mode!");
	}

	void SceneManager::exitPlayMode() {
		ECS_TRACE("SCENE_MANAGER: going to exit play mode");

		const auto& entitiesVector = m_scene->getEntities();
		const auto& collectionsVector = m_scene->getCollections();

		std::for_each(entitiesVector.cbegin(), entitiesVector.cend(), [&playStorage = m_playStorage](const Entity& entity) {
			playStorage.loadEntityFromStorage(entity);
		});

		std::for_each(collectionsVector.cbegin(), collectionsVector.cend(), [&playStorage = m_playStorage](const EntityCollection& collection) {
			playStorage.loadCollectionFromStorage(collection);
		});

		initialize();

		ECS_INFO("SCENE_MANAGER: exited play mode!");
	}

	void SceneManager::updatePlayMode() {
		ECS_TRACE("SCENE_MANAGER: going to update play mode");

		auto& renderPipeline = graphics::RenderPipeline::getInstance();
		const auto& entitiesVector = m_scene->getEntities();

		auto updateEntityWithScript = [&renderPipeline, this](const Entity& entity) {
			if (entity.hasComponent<ScriptComponent>()) {
				const auto& script = entity.getComponent<ScriptComponent>();
				script.pythonScript.update(entity);
				updateEntityInPlaymode(entity, renderPipeline);
			}

			submitCameraIfPossible(entity);
		};

		std::for_each(entitiesVector.cbegin(), entitiesVector.cend(), updateEntityWithScript);

		/// TODO: add support for collection scripting

		ECS_INFO("SCENE_MANAGER: updated play mode");
	}

	void SceneManager::updatePauseMode() {
		ECS_TRACE("SCENE_MANAGER: going to update pause mode");

		auto& renderPipeline = graphics::RenderPipeline::getInstance();
		const auto& entitiesVector = m_scene->getEntities();

		auto updateEntityWithScript = [&renderPipeline, this](const Entity& entity) {
			updateEntityInPlaymode(entity, renderPipeline);
			submitCameraIfPossible(entity);
		};

		std::for_each(entitiesVector.cbegin(), entitiesVector.cend(), updateEntityWithScript);

		/// TODO: add support for collection
		
		ECS_INFO("SCENE_MANAGER: updated pause mode");
	}

	void SceneManager::updateEntityInPlaymode(const Entity& entity, RenderPipeline& renderPipeline) {
		const auto& transform = entity.getComponent<TransformComponent>();
		const auto& rpc = entity.getComponent<RenderPipelineComponent>();

		if (entity.hasComponent<RenderableComponent>()) {
			renderPipeline.modifyTransform(transform, rpc.container_index, rpc.transform_index);
		}

		if (entity.hasComponent<LightComponent>()) {
			const auto& light = entity.getComponent<LightComponent>();
			renderPipeline.modifyLight(transform.center, light, rpc.container_light_index, rpc.light_index);
		}

		if (entity.hasComponent<ColorComponent>()) {
			const auto& color = entity.getComponent<ColorComponent>();
			renderPipeline.modifyColor(color, rpc.container_index, rpc.color_index);
		}
	}

	// -------------------------------------------------------------
	// CAMERA STUFF
	// -------------------------------------------------------------

	void SceneManager::submitCameraIfPossible(const Entity& entity) {
		if (entity.hasComponent<CameraComponent>()) {
			const auto& cam = entity.getComponent<CameraComponent>();
			const auto& transform = entity.getComponent<TransformComponent>();

			if (cam.id.find("main") == std::string::npos) { 
				return; 
			}

			auto& renderCamera = m_scene->getRenderCamera();
			renderCamera.calculateCameraTransforms(transform, cam);

			ECS_TRACE("SCENE_MANAGER: submitted camera");
		}
	}

	// -------------------------------------------------------------
	// GET / SET
	// -------------------------------------------------------------

	void SceneManager::setScene(Scene* scene) { 
		m_scene = scene; 
	}

	Scene* SceneManager::getScene() { 
		return m_scene; 
	}


}
