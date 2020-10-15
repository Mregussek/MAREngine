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


	void GUI_MainMenuBar::display() {
		display_mainMenuBar();

		{
			const char* new_file = "New Scene";
			if (m_newSceneWindow) {
				GUI_Filesystem::SetOpenNewScene(new_file);
				m_newSceneWindow = false;
			}
			GUI_Filesystem::Filesystem_NewScene(new_file);
		}
		{
			const char* open_file = "Open Scene";
			if (m_loadSceneWindow) {
				GUI_Filesystem::SetOpenLoadScene(open_file);
				m_loadSceneWindow = false;
			}
			GUI_Filesystem::Filesystem_LoadScene(open_file);
		}
		{
			const char* save_file = "Save Scene";
			if (m_saveSceneWindow) {
				GUI_Filesystem::SetOpenSaveScene(save_file);
				m_saveSceneWindow = false;
			}
			GUI_Filesystem::Filesystem_SaveScene(save_file, m_sceneManager->getScene());
		}
		{
			const char* load_obj = "OBJ Loader";
			if (m_loadOBJfileWindow) {
				GUI_Filesystem::SetOpenLoadOBJfile(load_obj);
				m_loadOBJfileWindow = false;
			}
			GUI_Filesystem::Filesystem_LoadOBJfile(load_obj, m_sceneManager->getScene());
		}
		{
			if (m_infoWindow) {
				GUI_Info::Menu_Info(m_infoWindow);
			}
			if (m_instructionWindow) {
				GUI_Info::Menu_Instruction(m_instructionWindow);
			}
		}
	}

	void GUI_MainMenuBar::display_mainMenuBar() {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("Scene")) {
				if (ImGui::MenuItem("New Scene")) {
					m_newSceneWindow = true;
				}

				if (ImGui::MenuItem("Open Scene")) {
					m_loadSceneWindow = true;
				}

				if (ImGui::MenuItem("Save Scene")) {
					m_saveSceneWindow = true;
				}

				if (ImGui::MenuItem("Exit")) {
					window::Window::getInstance().endRenderLoop();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Entities")) {
				if (ImGui::MenuItem("Load external .obj file")) {
					m_loadOBJfileWindow = true;
				}

				/*
				if (GUI_EntityPanel::currentEntity) {
					const char* shortcut = GUI_EntityPanel::currentEntity->getComponent<ecs::TagComponent>().tag.c_str();
					if (ImGui::MenuItem("Copy selected entity", shortcut)) {
						auto& entity = m_sceneManager->getScene()->createEntity();
						ecs::EntityOperation::copyEntity(*GUI_EntityPanel::currentEntity, entity);
						GUI_EntityPanel::currentEntity = &entity;
					}
				}

				if (GUI_EntityCollectionPanel::currentCollection && !GUI_EntityPanel::currentEntity) {
					const char* shortcut = GUI_EntityCollectionPanel::currentCollection->getComponent<ecs::TagComponent>().tag.c_str();
					if (ImGui::MenuItem("Copy selected collection", shortcut)) {
						auto& collection = m_sceneManager->getScene()->createCollection();
						ecs::EntityOperation::copyCollection(*GUI_EntityCollectionPanel::currentCollection, collection);
						GUI_EntityCollectionPanel::currentCollection = &collection;
					}
				}
				*/
				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("About")) {
				m_infoWindow = true;
				m_instructionWindow = true;
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
