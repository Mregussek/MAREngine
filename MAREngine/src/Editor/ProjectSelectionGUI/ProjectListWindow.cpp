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

		if (ImGui::Button("Open Default Project")) {
			engine::MAREngine::Instance()->setProjectName("DefaultProject");
			engine::MAREngine::Instance()->setProjectPath("DefaultProject/");
			engine::MAREngine::Instance()->setLoadPath("DefaultProject/Scenes/DefaultProject.marscene");
			window::endRenderLoop();
		}

		ImGui::SameLine();

		if (ImGui::Button("New Project")) { m_filesystemSelection.openWindowNewProject(); }
		m_filesystemSelection.windowNewProject();

		ImGui::SameLine();

		if (ImGui::Button("Open Project")) { m_filesystemSelection.openWindowOpenProject(); }
		m_filesystemSelection.windowOpenProject();

		ImGui::SameLine();

		if (ImGui::Button("Rename Project")) {

		}

		ImGui::SameLine();

		if (ImGui::Button("Delete Project")) {

		}

		if (ImGui::Button("Exit")) {
			window::endRenderLoop();
		}

		ImGui::End();

	}


}
