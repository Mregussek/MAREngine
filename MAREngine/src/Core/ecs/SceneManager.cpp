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
#include "../graphics/RenderAPI/RenderPipeline.h"


namespace mar::ecs {


	void SceneManager::initialize() const {
		ECS_TRACE("SCENE_MANAGER: going to initialize!");

		graphics::RenderPipeline::Instance->reset();

		const auto& entitiesVector = m_scene->getEntities();
		const auto& collectionsVector = m_scene->getCollections();

		auto pushEntityToPipeline = [](const Entity& entity) {
			graphics::RenderPipeline::Instance->submitEntity(entity);
		};

		auto pushCollectionToPipeline = [&pushEntityToPipeline](const EntityCollection& collection) {
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

	void SceneManager::setPlayMode() { 
		m_EditorMode = false; 
		initPlayMode(); 
		SceneEvents::Instance().onGameCameraSet(); 
	}

	void SceneManager::setExitPlayMode() { 
		m_EditorMode = true; 
		unsetPauseMode(); 
		exitPlayMode(); 
	}

	// -------------------------------------------------------------
	// EDITOR MODE
	// -------------------------------------------------------------

	void SceneManager::updateEditorMode() {

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

		const auto view = m_scene->getView<ScriptComponent>();
		std::for_each(view.begin(), view.end(), [this](entt::entity entt_entity) {
			const auto& script{ m_scene->getComponent<ScriptComponent>(entt_entity) };
			const Entity entity(entt_entity, m_scene->getRegistry());
			script.pythonScript.update(entity);
			updateEntityInPlaymode(entity);
		});

		ECS_INFO("SCENE_MANAGER: updated play mode");
	}

	void SceneManager::updatePauseMode() {
		ECS_TRACE("SCENE_MANAGER: going to update pause mode");

		const auto view = m_scene->getView<ScriptComponent>();
		std::for_each(view.begin(), view.end(), [this](entt::entity entt_entity) {
			const Entity entity(entt_entity, m_scene->getRegistry());
			updateEntityInPlaymode(entity);
		});

		ECS_INFO("SCENE_MANAGER: updated pause mode");
	}

	void SceneManager::updateEntityInPlaymode(const Entity& entity) {
		const auto& transform = entity.getComponent<TransformComponent>();
		const auto& rpc = entity.getComponent<RenderPipelineComponent>();

		if (entity.hasComponent<RenderableComponent>()) {
			SceneEvents::Instance().onTransformUpdate(entity);
		}

		if (entity.hasComponent<LightComponent>()) {
			SceneEvents::Instance().onLightUpdate(entity);
		}

		if (entity.hasComponent<ColorComponent>()) {
			SceneEvents::Instance().onColorUpdate(entity);
		}

		if (entity.hasComponent<CameraComponent>()) {
			const auto& camera{ entity.getComponent<CameraComponent>() };
			if (camera.checkIfMain()) { SceneEvents::Instance().onMainCameraUpdate(entity); }	
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
