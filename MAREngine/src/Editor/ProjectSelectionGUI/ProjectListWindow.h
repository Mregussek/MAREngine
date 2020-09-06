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


#include "../../mar.h"
#include "../../Window/Window.h"
#include "../../Engine.h"
#include "ProjectSelectionFilesystem.h"


namespace mar {
	namespace editor {


		class ProjectListWindow {
		public:

			static void display() {
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

				ImGui::Separator();

				if (ImGui::Button("New Project")) {
					ProjectSelectionFilesystem::openWindowNewProject("Create New Project");
				}
				ProjectSelectionFilesystem::windowNewProject();

				ImGui::SameLine();

				if (ImGui::Button("Open Project")) {
					ProjectSelectionFilesystem::openWindowOpenProject("Open Project Window");
				}
				ProjectSelectionFilesystem::windowOpenProject();

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
		};


} }
