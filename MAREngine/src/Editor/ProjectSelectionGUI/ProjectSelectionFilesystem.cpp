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


#include "ProjectSelectionFilesystem.h"


namespace mar {
	namespace editor {


		std::string ProjectSelectionFilesystem::projectPath{ "" };
		std::string ProjectSelectionFilesystem::projectName{ "" };
		bool ProjectSelectionFilesystem::new_project_name_selected{ false };
		bool ProjectSelectionFilesystem::open_existing_project{ false };


		void ProjectSelectionFilesystem::openWindowNewProject(const char* name) {
			igfd::ImGuiFileDialog::Instance()->OpenDialog(name, name, 0, ".");
		}

		void ProjectSelectionFilesystem::windowNewProject(const char* name) {
			static bool should_popup_be_opened = false;

			if (igfd::ImGuiFileDialog::Instance()->FileDialog(name)) {
				if (igfd::ImGuiFileDialog::Instance()->IsOk == true) {
					projectPath = igfd::ImGuiFileDialog::Instance()->GetFilePathName() + "/";
					projectName = igfd::ImGuiFileDialog::Instance()->GetCurrentFileName();
					should_popup_be_opened = true;
				}

				igfd::ImGuiFileDialog::Instance()->CloseDialog(name);
			}

			if (should_popup_be_opened) {
				ImGui::Begin("Select New Project Name");

				ImGui::Text("Project will be created at: %s", projectPath.c_str());

				static char name[100];
				ImGui::InputText("ProjectName: ", name, 100);

				if (ImGui::Button("Create")) {
					projectName = std::string(name);
					projectPath += projectName;
					new_project_name_selected = true;
					should_popup_be_opened = false;

					ImGui::End();
					return;
				}

				ImGui::SameLine();

				if (ImGui::Button("Cancel")) {
					projectPath = "";
					projectName = "";
					new_project_name_selected = false;
					should_popup_be_opened = false;

					ImGui::End();
					return;
				}

				ImGui::End();
			}
		}

		void ProjectSelectionFilesystem::openWindowOpenProject(const char* name) {
			igfd::ImGuiFileDialog::Instance()->OpenDialog(name, name, 0, ".");
		}

		void ProjectSelectionFilesystem::windowOpenProject(const char* name) {
			static bool should_popup_be_opened = false;

			if (igfd::ImGuiFileDialog::Instance()->FileDialog(name)) {
				if (igfd::ImGuiFileDialog::Instance()->IsOk == true) {
					projectPath = igfd::ImGuiFileDialog::Instance()->GetFilePathName() + "/";
					projectName = igfd::ImGuiFileDialog::Instance()->GetCurrentFileName();
					should_popup_be_opened = true;
				}

				igfd::ImGuiFileDialog::Instance()->CloseDialog(name);
			}

			if (should_popup_be_opened) {
				ImGui::Begin("Open Project Manager");

				ImGui::Text("MAREngine will look for project at: %s", projectPath.c_str());

				if (ImGui::Button("Open")) {
					open_existing_project = true;
					should_popup_be_opened = false;

					ImGui::End();
					return;
				}

				ImGui::SameLine();

				if (ImGui::Button("Cancel")) {
					projectPath = "";
					projectName = "";
					open_existing_project = false;
					should_popup_be_opened = false;

					ImGui::End();
					return;
				}

				ImGui::End();
			}
		}


} }