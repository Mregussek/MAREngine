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
			static void windowNewProject(const char* name);

			static void openWindowOpenProject(const char* name);
			static void windowOpenProject(const char* name);

			// ----------------------------------------------
			// STATE CHECKS
			// ----------------------------------------------

			static void checkState();

		};


} }


#endif // !MAR_ENGINE_EDITOR_PROJECT_SELECTION_FILESYSTEM_H