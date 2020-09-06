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


#ifndef MAR_ENGINE_EDITOR_PROJECT_SELECTION_FILESYSTEM_H
#define MAR_ENGINE_EDITOR_PROJECT_SELECTION_FILESYSTEM_H


#include "../../mar.h"
#include "../../Window/Window.h"
#include "../../Engine.h"
#include "../Filesystem/EditorFilesystem.h"


namespace mar {
	namespace editor {


		class ProjectSelectionFilesystem {
		public:
			
			// ----------------------------------------------
			// STATIC VARIABLES
			// ----------------------------------------------

			static std::string projectPath;
			static std::string projectName;
			static bool new_project_name_selected;
			static bool open_existing_project;

			// ----------------------------------------------
			// METHODS FOR PROJECT MANAGEMENT
			// ----------------------------------------------

			static void openWindowNewProject(const char* name);
			static void windowNewProject();

			static void openWindowOpenProject(const char* name);
			static void windowOpenProject();

			// ----------------------------------------------
			// STATE CHECKS
			// ----------------------------------------------

			static void checkState() {
				if (new_project_name_selected) {
#ifdef _WIN32
					std::string Assets = projectPath + "\\Assets";
					std::string Scenes = projectPath + "\\Scenes";
					std::string EmptySceneName = Scenes + "\\" + projectName + ".marscene";
#endif
#ifdef linux
					std::string Assets = projectPath + "/Assets";
					std::string Scenes = projectPath + "/Scenes";
					std::string EmptySceneName = Scenes + "/" + projectName + ".marscene";
#endif

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

					open_existing_project = false;
				}
			}

		};


} }


#endif // !MAR_ENGINE_EDITOR_PROJECT_SELECTION_FILESYSTEM_H