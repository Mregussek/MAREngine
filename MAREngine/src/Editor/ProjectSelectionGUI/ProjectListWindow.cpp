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


#include "ProjectListWindow.h"
#include "ProjectSelectionFilesystem.h"
#include "../../Window/Window.h"
#include "../../Engine.h"


namespace mar::editor {


	void ProjectListWindow::display() {
		ImGui::Begin("Project List");

		static const char* sort_types[2] = {
			"By Name",
			"By Modification Date"
		};

		static int32_t selected = 0;

		ImGui::Combo("Sort", &selected, sort_types, 2);

		ImGui::SameLine();

		static char input[100];
		ImGui::InputText("Search", input, 100);

		ImGui::Text("\n\n");

		ImGui::Separator();

		const char* open_default = "Open Default Project";
		if (ImGui::Button("Open Default Project")) {
			engine::MAREngine::getEngine()->setProjectName("DefaultProject");
			engine::MAREngine::getEngine()->setProjectPath("DefaultProject/");
			engine::MAREngine::getEngine()->setLoadPath("DefaultProject/Scenes/DefaultProject.marscene");
			window::Window::getInstance().endRenderLoop();
		}

		ImGui::SameLine();

		{
			const char* new_project_name = "Create New Project";
			if (ImGui::Button("New Project")) {
				ProjectSelectionFilesystem::openWindowNewProject(new_project_name);
			}
			ProjectSelectionFilesystem::windowNewProject(new_project_name);
		}

		ImGui::SameLine();

		{
			const char* open_project_name = "Open Project Window";
			if (ImGui::Button("Open Project")) {
				ProjectSelectionFilesystem::openWindowOpenProject(open_project_name);
			}
			ProjectSelectionFilesystem::windowOpenProject(open_project_name);
		}

		ImGui::SameLine();

		if (ImGui::Button("Rename Project")) {

		}

		ImGui::SameLine();

		if (ImGui::Button("Delete Project")) {

		}

		if (ImGui::Button("Exit")) {
			window::Window::getInstance().exitApp();
		}

		ImGui::End();

		ProjectSelectionFilesystem::checkState();
	}


}
