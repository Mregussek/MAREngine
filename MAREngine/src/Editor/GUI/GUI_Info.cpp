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


#include "GUI_Info.h"
#include "../EditorLogging.h"


namespace mar {
	namespace editor {


		void GUI_Info::Menu_Info(bool& should_be_open) {
			ImGui::Begin("About");

			const char* aboutEngine = "MAREngine is a AR engine for abstracting 3D models in real world.";
			const char* aboutAuthor = "Mateusz Rzeczyca is C++ / Python programmer and ethusiast of Augmented Reality && Machine Learning.\nHe is a student of Eletronics and Telecommunications at AGH University of Science and Technology";

			ImGui::Text("About Engine");
			ImGui::Text(aboutEngine);

			ImGui::Separator();

			ImGui::Text("About Author");
			ImGui::Text(aboutAuthor);

			if (ImGui::Button("Close")) {
				should_be_open = false;
			}

			ImGui::End();

			EDITOR_TRACE("GUI: menu_info");
		}

		void GUI_Info::Menu_Instruction(bool& should_be_open) {
			ImGui::Begin("Instructions");

			const char* fileManage = "File Management";
			const char* aboutFileManageSave = "Save Method:\nYou have to give the name of file, where you want to write current scene.\nIf file exists, it will be truncated and new scene will be put there.";
			const char* aboutFileManageOpen = "Open Method:\nMethod searches all files in directory resources/mar_files and gets all scenes.\nYou have to click reload button in order to see current state of directory.\nThen you choose one path, which will be loaded into MAREngine.";

			ImGui::Text(fileManage);
			ImGui::Text(aboutFileManageSave);
			ImGui::Text(aboutFileManageOpen);
			ImGui::Separator();

			const char* keyboardSettings = "Keyboard Settings";
			const char* aboutKeySettings = "If Viewport window is selected you can move in scene using WASD keys.\nIf you click Q key, then mouse usage will be enabled! Press it again to disable mouse!";

			ImGui::Text(keyboardSettings);
			ImGui::Text(aboutKeySettings);
			ImGui::Separator();

			const char* mouseSettings = "Mouse Settings";
			const char* aboutMouseSettings = "If you have clicked Q key, you can move around with mouse. Scroll is also available to see things.";

			ImGui::Text(mouseSettings);
			ImGui::Text(aboutMouseSettings);
			ImGui::Separator();

			const char* addShape = "Adding Shapes to Scene";
			const char* aboutAddShape = "In order to add shape to scene, you need to give center of the object. Next point is to select texture.\nIf empty is chosen, shape will be rendered with its default color!";

			ImGui::Text(addShape);
			ImGui::Text(aboutAddShape);
			ImGui::Separator();

			if (ImGui::Button("Close")) {
				should_be_open = false;
			}

			ImGui::End();

			EDITOR_TRACE("GUI: menu_instruction");
		}


} }