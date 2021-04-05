/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#include "SceneManagerEditor.h"
#include "Scene.h"
#include "Entity/EventsComponentEntity.h"
#include "Entity/EventsCameraEntity.h"
#include "../graphics/Renderer/RenderManager.h"
//#include "../graphics/RenderAPI/RenderPipeline.h"


namespace marengine {


	void FSceneManagerEditor::initialize(Scene* pScene, FRenderManager* pRenderManager) {
	    m_pScene = pScene;
		m_pRenderManager = pRenderManager;
		pushSceneToPipeline();
	}

    void FSceneManagerEditor::pushSceneToPipeline() {
		m_pRenderManager->pushSceneToRender(m_pScene);
        //RenderPipeline::Instance->create(getScene());
	}

	void FSceneManagerEditor::update() {
		if (isPlayMode()) {
			if (isPauseMode()) {
				updatePauseMode();
			}
			else {
				updatePlayMode();
			}
		}
	}

	void FSceneManagerEditor::close() {
		m_pScene->close(); 
		delete m_pScene;
	}

	void FSceneManagerEditor::initPlayMode() {
		const FEntityArray& entities{ m_pScene->getEntities() };
		for (const Entity& entity : entities) {
			FScenePlayStorage::pushEntityToStorage(entity);
		}

		auto initializeScriptModule = [this](entt::entity entt_entity, PythonScriptComponent& script) {
			const Entity entity(entt_entity, m_pScene->getRegistry());
			script.pythonScript.loadScript(script.scriptsPath);
			script.pythonScript.start(entity);
		};

		const auto view{ m_pScene->getView<PythonScriptComponent>() };
		view.each(initializeScriptModule);

		FEventsCameraEntity::onGameCameraSet();
	}

	void FSceneManagerEditor::updatePlayMode() {
		auto updateScriptModule = [this](entt::entity entt_entity, PythonScriptComponent& script) {
			const Entity entity(entt_entity, m_pScene->getRegistry());
			script.pythonScript.update(entity);
			updateEntityInPlaymode(entity);
		};

		const auto view{ m_pScene->getView<PythonScriptComponent>() };
		view.each(updateScriptModule);
	}

	void FSceneManagerEditor::updatePauseMode() {
		const auto view = m_pScene->getView<PythonScriptComponent>();
		view.each([this](entt::entity entt_entity, const PythonScriptComponent& script) {
			const Entity entity(entt_entity, m_pScene->getRegistry());
			updateEntityInPlaymode(entity);
		});
	}

	void FSceneManagerEditor::updateEntityInPlaymode(const Entity& entity) {
		FEventsComponentEntity::onUpdate<TransformComponent>(entity);

		if (entity.hasComponent<PointLightComponent>()) {
			FEventsComponentEntity::onUpdate<PointLightComponent>(entity);
		}

		if (entity.hasComponent<ColorComponent>()) {
			FEventsComponentEntity::onUpdate<ColorComponent>(entity);
		}
	}

	void FSceneManagerEditor::exitPlayMode() {
		const FEntityArray& entities{ m_pScene->getEntities() };

		for (const Entity& entity : entities) {
			FScenePlayStorage::loadEntityFromStorage(entity);
		}

        pushSceneToPipeline();
	}

	Scene* FSceneManagerEditor::getScene() { 
		return m_pScene; 
	}

	void FSceneManagerEditor::setEditorMode() { 
		m_EditorMode = true; 
	}

	bool FSceneManagerEditor::isEditorMode() const {
		return m_EditorMode;
	}

	void FSceneManagerEditor::setPlayMode() {
		m_EditorMode = false;
		initPlayMode();
	}

	bool FSceneManagerEditor::isPlayMode() const {
		return !m_EditorMode;
	}

	void FSceneManagerEditor::setExitPlayMode() {
		m_EditorMode = true;
		setExitPauseMode();
		exitPlayMode();
	}

	void FSceneManagerEditor::setPauseMode() { 
		m_PauseMode = true;
	}

	bool FSceneManagerEditor::isPauseMode() const { 
		return m_PauseMode; 
	}

	void FSceneManagerEditor::setExitPauseMode() { 
		m_PauseMode = false; 
	}

	void FSceneManagerEditor::useEditorCamera() {
		m_EditorCamera = true;
	}

	void FSceneManagerEditor::useGameCamera() {
		FEventsCameraEntity::onGameCameraSet();
		m_EditorCamera = false;
	}

	bool FSceneManagerEditor::usingEditorCamera() const {
		return m_EditorCamera;
	}

	bool FSceneManagerEditor::usingGameCamera() const {
		return !m_EditorCamera;
	}


}
