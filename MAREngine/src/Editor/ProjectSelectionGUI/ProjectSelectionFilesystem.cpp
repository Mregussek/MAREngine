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


	void ProjectSelectionFilesystem::openWindowNewProject() {
		ImGui::OpenPopup(m_nameNewProject);
	}

	void ProjectSelectionFilesystem::windowNewProject() {
		if (m_fileDialog.showFileDialog(m_nameNewProject, imgui_addons::ImGuiFileBrowser::DialogMode::SELECT, ImVec2(1200, 800), "")) {

		}
	}

	void ProjectSelectionFilesystem::openWindowOpenProject() {
		ImGui::OpenPopup(m_nameOpenProject);
	}

	void ProjectSelectionFilesystem::windowOpenProject() {
		if (m_fileDialog.showFileDialog(m_nameOpenProject, imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(1200, 800), ".marscene")) {

		}
	}

	/*
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
	*/


}
