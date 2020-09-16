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


		void GUI_Filesystem::SetOpenNewScene(const char* name) {
			std::string scenesPath = engine::MAREngine::getEngine()->getProjectPath() + "/Scenes";
			igfd::ImGuiFileDialog::Instance()->OpenDialog(name, name, ".marscene", scenesPath);
			igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".marscene", ImVec4(0, 1, 0, 0.9f));
		}

		void GUI_Filesystem::Filesystem_NewScene(const char* name) {
			if (igfd::ImGuiFileDialog::Instance()->FileDialog(name)) {
				if (igfd::ImGuiFileDialog::Instance()->IsOk == true) {



				}

				igfd::ImGuiFileDialog::Instance()->CloseDialog(name);
			}
		}

		void GUI_Filesystem::SetOpenSaveScene(const char* name) {
			std::string scenesPath = engine::MAREngine::getEngine()->getProjectPath() + "/Scenes";
			igfd::ImGuiFileDialog::Instance()->OpenDialog(name, name, ".marscene", scenesPath);
			igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".marscene", ImVec4(0, 1, 0, 0.9f));
		}

		void GUI_Filesystem::Filesystem_SaveScene(const char* name, ecs::Scene* scene_to_save) {
			if (igfd::ImGuiFileDialog::Instance()->FileDialog(name)) {
				if (igfd::ImGuiFileDialog::Instance()->IsOk == true) {

					std::string filePathName = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
					Filesystem::saveToFile(scene_to_save, filePathName.c_str());

				}

				igfd::ImGuiFileDialog::Instance()->CloseDialog(name);
			}
		}

		void GUI_Filesystem::SetOpenLoadScene(const char* name) {
			std::string scenesPath = engine::MAREngine::getEngine()->getProjectPath() + "/Scenes";
			igfd::ImGuiFileDialog::Instance()->OpenDialog(name, name, ".marscene", scenesPath);
			igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".marscene", ImVec4(0, 1, 0, 0.9f));
		}

		void GUI_Filesystem::Filesystem_LoadScene(const char* name) {
			if (igfd::ImGuiFileDialog::Instance()->FileDialog(name)) {
				if (igfd::ImGuiFileDialog::Instance()->IsOk == true) {

					engine::MAREngine::getEngine()->setLoadPath(igfd::ImGuiFileDialog::Instance()->GetFilePathName());

					GUI_EntityCollectionPanel::reset();
					GUI_EntityPanel::reset();
					GUI_TextEditor::Instance().reset();

					engine::MAREngine::getEngine()->setRestart();

				}

				igfd::ImGuiFileDialog::Instance()->CloseDialog(name);
			}
		}

		void GUI_Filesystem::SetOpenLoadOBJfile(const char* name) {
			std::string assetsPath = engine::MAREngine::getEngine()->getProjectPath() + "/Assets";
			igfd::ImGuiFileDialog::Instance()->OpenDialog(name, name, ".obj", assetsPath);
			igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".obj", ImVec4(0, 1, 0, 0.9f));
		}

		void GUI_Filesystem::Filesystem_LoadOBJfile(const char* name, ecs::Scene* scene) {
			if (igfd::ImGuiFileDialog::Instance()->FileDialog(name)) {
				if (igfd::ImGuiFileDialog::Instance()->IsOk == true) {

					std::string filePathName = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
					std::string filename = igfd::ImGuiFileDialog::Instance()->GetCurrentFileName();

					auto& collection = scene->createCollection();
					auto& tag = collection.getComponent<ecs::TagComponent>();
					auto& crc = collection.addComponent<ecs::CollectionRenderableComponent>();

					tag.tag = filename;
					crc = filePathName;

					graphics::MeshCreator::loadOBJ(filename, filePathName, collection);
					ecs::SceneEvents::Instance().onCollectionOBJloaded(collection);

				}

				igfd::ImGuiFileDialog::Instance()->CloseDialog(name);
			}	
		}

		void GUI_Filesystem::Filesystem_AssetManager(const char* name) {

		}



} }