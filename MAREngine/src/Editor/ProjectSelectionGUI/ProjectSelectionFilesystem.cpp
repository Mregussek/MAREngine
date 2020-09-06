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
			igfd::ImGuiFileDialog::Instance()->OpenDialog("ChooseDirDlgKey", name, 0, ".");
		}

		void ProjectSelectionFilesystem::windowNewProject() {
			static bool should_popup_be_opened = false;

			if (igfd::ImGuiFileDialog::Instance()->FileDialog("ChooseDirDlgKey")) {
				if (igfd::ImGuiFileDialog::Instance()->IsOk == true) {

#ifdef _WIN32 
					projectPath = igfd::ImGuiFileDialog::Instance()->GetFilePathName() + "\\";
#endif
#ifdef linux
					projectPath = igfd::ImGuiFileDialog::Instance()->GetFilePathName() + "/";
#endif
					ImGui::OpenPopup("Select Project Name");
					should_popup_be_opened = true;

				}

				igfd::ImGuiFileDialog::Instance()->CloseDialog("ChooseDirDlgKey");
			}

			if (should_popup_be_opened) {
				if (ImGui::BeginPopupModal("Select Project Name")) {

					ImGui::Text("Project will be created at: %s", projectPath.c_str());

					static char name[100];
					ImGui::InputText("ProjectName: ", name, 100);

					if (ImGui::Button("Create")) {
						projectName = std::string(name);
						projectPath += projectName;
						new_project_name_selected = true;
						should_popup_be_opened = false;

						ImGui::CloseCurrentPopup();
					}

					ImGui::SameLine();

					if (ImGui::Button("Cancel")) {
						projectPath = "";
						projectName = "";
						new_project_name_selected = false;
						should_popup_be_opened = false;

						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}
		}

		void ProjectSelectionFilesystem::openWindowOpenProject(const char* name) {
			igfd::ImGuiFileDialog::Instance()->OpenDialog("ChooseDirDlgKey", name, 0, ".");
		}

		void ProjectSelectionFilesystem::windowOpenProject() {
			static bool should_popup_be_opened = false;

			if (igfd::ImGuiFileDialog::Instance()->FileDialog("ChooseDirDlgKey")) {
				if (igfd::ImGuiFileDialog::Instance()->IsOk == true) {

#ifdef _WIN32 
					projectPath = igfd::ImGuiFileDialog::Instance()->GetFilePathName() + "\\";
#endif
#ifdef linux
					projectPath = igfd::ImGuiFileDialog::Instance()->GetFilePathName() + "/";
#endif
					projectName = igfd::ImGuiFileDialog::Instance()->GetCurrentFileName();
					ImGui::OpenPopup("Are you sure it is correct path");
					should_popup_be_opened = true;
				}

				igfd::ImGuiFileDialog::Instance()->CloseDialog("ChooseDirDlgKey");
			}

			if (should_popup_be_opened) {
				if (ImGui::BeginPopupModal("Are you sure it is correct path")) {

					ImGui::Text("MAREngine will look for project at: %s", projectPath.c_str());

					if (ImGui::Button("Open")) {
						std::cout << projectPath << std::endl;
						std::cout << projectName << std::endl;
						open_existing_project = true;
						should_popup_be_opened = false;

						ImGui::CloseCurrentPopup();
					}

					ImGui::SameLine();

					if (ImGui::Button("Cancel")) {
						projectPath = "";
						projectName = "";
						open_existing_project = false;
						should_popup_be_opened = false;

						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}
		}


} }