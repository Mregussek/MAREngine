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

		}

		void GUI_Filesystem::Filesystem_NewScene(const char* name) {
			
		}

		void GUI_Filesystem::SetOpenSaveScene(const char* name) {

		}

		void GUI_Filesystem::Filesystem_SaveScene(const char* name, ecs::Scene* scene_to_save) {
			//Filesystem::saveToFile(scene_to_save, s_fileDialog.selected_path.c_str());
		}

		void GUI_Filesystem::SetOpenLoadScene(const char* name) {
			igfd::ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", name, ".marscene", ".");
			igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".marscene", ImVec4(0, 1, 0, 0.9));
		}

		void GUI_Filesystem::Filesystem_LoadScene(const char* name) {
			if (igfd::ImGuiFileDialog::Instance()->FileDialog("ChooseFileDlgKey")) {
				if (igfd::ImGuiFileDialog::Instance()->IsOk == true) {
					engine::MAREngine::getEngine()->setLoadPath(igfd::ImGuiFileDialog::Instance()->GetFilePathName());

					GUI_EntityCollectionPanel::reset();
					GUI_EntityPanel::reset();
					GUI_TextEditor::Instance().reset();

					engine::MAREngine::getEngine()->setRestart();
				}

				igfd::ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
			}
		}

		void GUI_Filesystem::SetOpenLoadOBJfile(const char* name) {

		}

		void GUI_Filesystem::Filesystem_LoadOBJfile(const char* name, ecs::Scene* scene) {

				/*
				auto& collection = scene->createCollection();
				auto& tag = collection.getComponent<ecs::TagComponent>();
				tag.tag = s_fileDialog.selected_fn;
				
				graphics::MeshCreator::loadOBJ(s_fileDialog.selected_fn, s_fileDialog.selected_path, collection);
				ecs::SceneEvents::Instance().onCollectionOBJloaded(collection);
				*/
			
		}

		void GUI_Filesystem::Filesystem_AssetManager(const char* name) {

		}



} }