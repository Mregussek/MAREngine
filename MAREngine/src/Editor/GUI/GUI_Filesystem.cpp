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


#include "GUI_Filesystem.h"

#include "GUI_TextEditor.h"
#include "GUI_EntityPanel.h"
#include "GUI_EntityCollectionPanel.h"


namespace mar {
	namespace editor {


		imgui_addons::ImGuiFileBrowser GUI_Filesystem::s_fileDialog;
		imgui_addons::ImGuiFileBrowser GUI_Filesystem::s_windowDialog;


		void GUI_Filesystem::Filesystem_NewScene(const char* name) {
			if (s_fileDialog.showFileDialog(name, imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(1000, 510), ".marscene,.mrsc")) {
				GUI_EntityCollectionPanel::reset();
				GUI_EntityPanel::reset();
				GUI_TextEditor::Instance().reset();
				
				engine::MAREngine::getEngine()->setLoadPath("BrandNewScene");
				engine::MAREngine::getEngine()->setRestart();
			}

			EDITOR_TRACE("GUI: Filesystem_NewScene");
		}

		void GUI_Filesystem::Filesystem_SaveScene(const char* name, ecs::Scene* scene_to_save) {
			if (s_fileDialog.showFileDialog(name, imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(1000, 510), ".marscene,.mrsc")) {
				if (s_fileDialog.selected_path.find(".marscene") != std::string::npos) {
					Filesystem::saveToFile(scene_to_save, s_fileDialog.selected_path.c_str());
				}
				else if (s_fileDialog.selected_path.find(".mrsc") != std::string::npos) {
					Filesystem::saveToFile(scene_to_save, s_fileDialog.selected_path.c_str());
				}
				else {
					EDITOR_ERROR("GUI_FILESYSTEM: given path {} has no correct extension, expected ones are .marscene and .mrsc!", s_fileDialog.selected_path)
				}
			}
		}

		void GUI_Filesystem::Filesystem_LoadScene(const char* name) {
			if (s_fileDialog.showFileDialog(name, imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(1000, 510), ".marscene,.mrsc")) {
				engine::MAREngine::getEngine()->setLoadPath(s_fileDialog.selected_path);

				GUI_EntityCollectionPanel::reset();
				GUI_EntityPanel::reset();
				GUI_TextEditor::Instance().reset();

				engine::MAREngine::getEngine()->setRestart();
			}
		}

		void GUI_Filesystem::Filesystem_LoadOBJfile(const char* name, ecs::Scene* scene) {
			if (s_fileDialog.showFileDialog(name, imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(1000, 510), ".obj")) {
				auto& collection = scene->createCollection();
				auto& tag = collection.getComponent<ecs::TagComponent>();
				tag.tag = s_fileDialog.selected_fn;
				
				graphics::MeshCreator::loadOBJ(s_fileDialog.selected_fn, s_fileDialog.selected_path, collection);
				ecs::SceneEvents::Instance().onCollectionOBJloaded(collection);
			}
		}

		void GUI_Filesystem::Filesystem_AssetManager(const char* name) {
			s_windowDialog.showFileWindow(name, imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(500, 250), ".obj,.jpg,.png");
		}



} }