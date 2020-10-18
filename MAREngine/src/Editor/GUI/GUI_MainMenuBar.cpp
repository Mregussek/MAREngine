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


#include "GUI_MainMenuBar.h"

#include "../EditorLogging.h"

#include "../../Window/Window.h"

#include "../../Core/ecs/Entity/Entity.h"
#include "../../Core/ecs/Entity/EntityCollection.h"
#include "../../Core/ecs/Entity/EntityOperation.h"
#include "../../Core/ecs/Components/Components.h"
#include "../../Core/ecs/SceneManager.h"
#include "../../Core/ecs/Scene.h"

#include "EntityPanels/GUI_EntityCollectionPanel.h"
#include "EntityPanels/GUI_EntityPanel.h"
#include "Other/GUI_Info.h"
#include "GUI_Filesystem.h"


namespace mar::editor {


	void GUI_MainMenuBar::initialize() {
		m_guiFilesystem.initialize();
	}

	void GUI_MainMenuBar::display() {
		display_mainMenuBar();

		if (m_newSceneWindow) {
			m_guiFilesystem.openNewSceneWindow();
			m_newSceneWindow = false;
		}
		m_guiFilesystem.displayNewSceneWindow();

		if (m_saveSceneWindow) {
			m_guiFilesystem.openSaveSceneWindow();
			m_saveSceneWindow = false;
		}
		m_guiFilesystem.displaySaveSceneWindow(m_sceneManager->getScene());

		if (m_loadSceneWindow) {
			m_guiFilesystem.openLoadSceneWindow();
			m_loadSceneWindow = false;
		}
		m_guiFilesystem.displayLoadSceneWindow();

		if (m_loadOBJfileWindow) {
			m_guiFilesystem.openLoadOBJWindow();
			m_loadOBJfileWindow = false;
		}
		m_guiFilesystem.displayLoadOBJWindow(m_sceneManager->getScene());

		if (m_infoWindow) {
			m_infoWindow = GUI_Info::InfoWindow();
		}
		if (m_instructionWindow) {
			m_instructionWindow = GUI_Info::InstructionWindow();
		}
	}

	void GUI_MainMenuBar::display_mainMenuBar() {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("Scene")) {
				if (ImGui::MenuItem("New Scene")) { m_newSceneWindow = true; }
				if (ImGui::MenuItem("Open Scene")) { m_loadSceneWindow = true; }
				if (ImGui::MenuItem("Save Scene")) { m_saveSceneWindow = true; }
				if (ImGui::MenuItem("Exit")) {
					window::Window::getInstance().endRenderLoop();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Entities")) {
				if (ImGui::MenuItem("Load external .obj file")) { m_loadOBJfileWindow = true; }

				const auto& currentEntity = GUI_EntityPanel::Instance()->getCurrentEntity();
				const bool entityExists = &currentEntity != nullptr;

				if (entityExists) {
					const char* shortcut = currentEntity.getComponent<ecs::TagComponent>().tag.c_str();
					if (ImGui::MenuItem("Copy selected entity", shortcut)) {
						const auto& entity = m_sceneManager->getScene()->createEntity();
						ecs::EntityOperation::copyEntity(currentEntity, entity);
						GUI_EntityPanel::Instance()->setCurrentEntity(entity);
					}
				}

				const auto& currentCollection = GUI_EntityCollectionPanel::Instance()->getCurrentCollection();
				const bool collectionExists = &currentCollection != nullptr;

				if (collectionExists && !entityExists) {
					const char* shortcut = currentCollection.getComponent<ecs::TagComponent>().tag.c_str();
					if (ImGui::MenuItem("Copy selected collection", shortcut)) {
						const auto& collection = m_sceneManager->getScene()->createCollection();
						ecs::EntityOperation::copyCollection(currentCollection, collection);
						GUI_EntityCollectionPanel::Instance()->setCurrentCollection(collection);
					}
				}
				
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("About")) {
				if (ImGui::MenuItem("About Engine")) { m_infoWindow = true; }
				if (ImGui::MenuItem("Instruction")) { m_instructionWindow = true; }

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Exit")) {
				window::Window::getInstance().endRenderLoop();
			}

			ImGui::EndMainMenuBar();
		}

		EDITOR_TRACE("GUI: pushing main menu bar");
	}

	void GUI_MainMenuBar::setSceneManager(ecs::SceneManager* manager) {
		m_sceneManager = manager;
	}


}
