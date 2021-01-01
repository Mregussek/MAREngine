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
#include "Scene.h"
#include "../events/SceneEvents.h"
#include "../events/RenderEvents.h"
#include "../graphics/RenderAPI/RenderPipeline.h"


namespace marengine {


	SceneManager* SceneManager::Instance{ nullptr };


	void SceneManager::initialize() const {
		ECS_TRACE("SCENE_MANAGER: going to initialize!");

		RenderPipeline::Instance->reset();

		const auto& entitiesVector = m_scene->getEntities();

		std::for_each(entitiesVector.cbegin(), entitiesVector.cend(), [](const Entity& entity) {
			RenderPipeline::Instance->pushEntityToPipeline(entity);
		});

		const auto view{ m_scene->getView<LightComponent>() };
		view.each([this](entt::entity entt_entity, const LightComponent& light) {
			const Entity entity(entt_entity, m_scene->getRegistry());
			RenderPipeline::Instance->pushLightToPipeline(entity);
		});

		RenderEvents::Instance().onContainersReadyToDraw();

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

		const auto& entitiesVector{ m_scene->getEntities() };

		auto saveEntityAtStorage = [&playStorage = m_playStorage](const Entity& entity) {
			playStorage.pushEntityToStorage(entity);
		};

		std::for_each(entitiesVector.cbegin(), entitiesVector.cend(), saveEntityAtStorage);

		auto initializeScriptModule = [this](entt::entity entt_entity, ScriptComponent& script) {
			const Entity entity(entt_entity, m_scene->getRegistry());
			script.pythonScript.loadScript(script.script);
			script.pythonScript.start(entity);
		};

		const auto view{ m_scene->getView<ScriptComponent>() };
		view.each(initializeScriptModule);

		ECS_INFO("SCENE_MANAGER: initialized play mode!");
	}

	void SceneManager::exitPlayMode() {
		ECS_TRACE("SCENE_MANAGER: going to exit play mode");

		const auto& entitiesVector{ m_scene->getEntities() };

		std::for_each(entitiesVector.cbegin(), entitiesVector.cend(), [&playStorage = m_playStorage](const Entity& entity) {
			playStorage.loadEntityFromStorage(entity);
		});

		initialize();

		ECS_INFO("SCENE_MANAGER: exited play mode!");
	}

	void SceneManager::updatePlayMode() {
		ECS_TRACE("SCENE_MANAGER: going to update play mode");

		auto updateScriptModule = [this](entt::entity entt_entity, ScriptComponent& script) {
			const Entity entity(entt_entity, m_scene->getRegistry());
			script.pythonScript.update(entity);
			updateEntityInPlaymode(entity);
		};

		const auto view{ m_scene->getView<ScriptComponent>() };
		view.each(updateScriptModule);

		ECS_INFO("SCENE_MANAGER: updated play mode");
	}

	void SceneManager::updatePauseMode() {
		ECS_TRACE("SCENE_MANAGER: going to update pause mode");

		const auto view = m_scene->getView<ScriptComponent>();
		view.each([this](entt::entity entt_entity, const ScriptComponent& script) {
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
