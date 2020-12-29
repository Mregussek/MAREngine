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


#include "MainMenuBarWidget.h"

#include "../../EditorLogging.h"

#include "../../../Window/Window.h"

#include "../../../Core/ecs/Entity/Entity.h"
#include "../../../Core/ecs/Entity/EntityCollection.h"
#include "../../../Core/ecs/Entity/EntityOperation.h"
#include "../../../Core/ecs/Components/Components.h"
#include "../../../Core/ecs/SceneManager.h"
#include "../../../Core/ecs/Scene.h"
#include "../../../Core/events/SceneEvents.h"

#include "../EntityPanels/EntityCollectionWidgetPanel.h"
#include "../EntityPanels/EntityWidgetPanel.h"
#include "../OtherPanels/InfoWidget.h"
#include "../Filesystem/FilesystemWidgets.h"
#include "../../WidgetEvents/EntityWidgetEvents.h"
#include "../../WidgetEvents/EntityCollectionWidgetEvents.h"


namespace marengine {


	void WMainMenuBarWidget::updateFrame() {
		displayActualMenuBar();
		handleInputs();
		
		EDITOR_TRACE("GUI: pushing main menu bar");
	}

	void WMainMenuBarWidget::displayActualMenuBar() {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("Scene")) {
				if (ImGui::MenuItem("New Scene")) { m_newSceneWindow = true; }
				if (ImGui::MenuItem("Open Scene")) { m_loadSceneWindow = true; }
				if (ImGui::MenuItem("Save Scene")) { m_saveSceneWindow = true; }
				if (ImGui::MenuItem("Exit")) {
					Window::endRenderLoop();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Entities")) {
				if (ImGui::MenuItem("Load external .obj file")) { m_loadOBJfileWindow = true; }

				const auto& currentEntity = WEntityWidgetPanel::Instance->getCurrentEntity();
				const bool entityExists = &currentEntity != nullptr;

				if (entityExists) {
					const char* shortcut = currentEntity.getComponent<TagComponent>().tag.c_str();
					if (ImGui::MenuItem("Copy selected entity", shortcut)) {
						FEntityWidgetEvents::onCopyEntity(currentEntity);
					}
				}

				const auto& currentCollection = WEntityCollectionWidgetPanel::Instance->getCurrentCollection();
				const bool collectionExists = &currentCollection != nullptr;

				if (collectionExists && !entityExists) {
					const char* shortcut = currentCollection.getComponent<TagComponent>().tag.c_str();
					if (ImGui::MenuItem("Copy selected collection", shortcut)) {
						FEntityCollectionWidgetEvents::onCopyCollection(currentCollection);
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
				Window::endRenderLoop();
			}

			ImGui::EndMainMenuBar();
		}
	}

	void WMainMenuBarWidget::handleInputs() {
		if (m_newSceneWindow) {
			WFilesystemWidgets::Instance->openNewSceneWindow();
			m_newSceneWindow = false;
		}
		if (m_saveSceneWindow) {
			WFilesystemWidgets::Instance->openSaveSceneWindow();
			m_saveSceneWindow = false;
		}
		if (m_loadSceneWindow) {
			WFilesystemWidgets::Instance->openLoadSceneWindow();
			m_loadSceneWindow = false;
		}
		if (m_loadOBJfileWindow) {
			WFilesystemWidgets::Instance->openLoadOBJWindow();
			m_loadOBJfileWindow = false;
		}
		if (m_infoWindow) { 
			WInfoWidget::Instance->displayInfo(); 
			m_infoWindow = false;
		}
		if (m_instructionWindow) { 
			WInfoWidget::Instance->displayInstruction(); 
			m_instructionWindow = false;
		}
	}


}
