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
#include "../../Window/Window.h"
#include "../../Engine.h"
#include "../Filesystem/EditorFilesystem.h"


namespace mar::editor {


	std::string ProjectSelectionFilesystem::projectPath{ "" };
	std::string ProjectSelectionFilesystem::projectName{ "" };
	bool ProjectSelectionFilesystem::new_project_name_selected{ false };
	bool ProjectSelectionFilesystem::open_existing_project{ false };


	void ProjectSelectionFilesystem::openWindowNewProject(const char* name) {
		//igfd::ImGuiFileDialog::Instance()->OpenDialog(name, name, 0, ".");
	}

	void ProjectSelectionFilesystem::windowNewProject(const char* name) {
		static bool should_popup_be_opened = false;

		/*
		if (igfd::ImGuiFileDialog::Instance()->FileDialog(name)) {
			if (igfd::ImGuiFileDialog::Instance()->IsOk == true) {
				projectPath = igfd::ImGuiFileDialog::Instance()->GetFilePathName() + "/";
				projectName = igfd::ImGuiFileDialog::Instance()->GetCurrentFileName();
				should_popup_be_opened = true;
			}

			igfd::ImGuiFileDialog::Instance()->CloseDialog(name);
		}
		*/

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
		//igfd::ImGuiFileDialog::Instance()->OpenDialog(name, name, 0, ".");
	}

	void ProjectSelectionFilesystem::windowOpenProject(const char* name) {
		/*
		if (igfd::ImGuiFileDialog::Instance()->FileDialog(name)) {
			if (igfd::ImGuiFileDialog::Instance()->IsOk == true) {
				projectPath = igfd::ImGuiFileDialog::Instance()->GetFilePathName() + "/";
				projectName = igfd::ImGuiFileDialog::Instance()->GetCurrentFileName();
				open_existing_project = true;
			}

			igfd::ImGuiFileDialog::Instance()->CloseDialog(name);
		}
		*/
	}

	void ProjectSelectionFilesystem::checkState() {
		if (new_project_name_selected) {
			std::string Assets = projectPath + "/Assets";
			std::string Scenes = projectPath + "/Scenes";
			std::string EmptySceneName = Scenes + "/" + projectName + ".marscene";

			std::filesystem::create_directories(Assets);
			std::filesystem::create_directories(Scenes);

			auto scene = ecs::Scene::createEmptyScene(projectName);
			Filesystem::saveToFile(scene, EmptySceneName.c_str());
			delete scene;

			engine::MAREngine::getEngine()->setProjectPath(projectPath);
			engine::MAREngine::getEngine()->setProjectName(projectName);
			engine::MAREngine::getEngine()->setLoadPath(EmptySceneName);
			window::Window::getInstance().endRenderLoop();

			new_project_name_selected = false;
		}

		if (open_existing_project) {
			std::string scene_to_load = projectPath + "/Scenes/" + projectName + ".marscene";

			engine::MAREngine::getEngine()->setProjectPath(projectPath);
			engine::MAREngine::getEngine()->setProjectName(projectName);
			engine::MAREngine::getEngine()->setLoadPath(scene_to_load);
			window::Window::getInstance().endRenderLoop();

			open_existing_project = false;
		}
	}


}
