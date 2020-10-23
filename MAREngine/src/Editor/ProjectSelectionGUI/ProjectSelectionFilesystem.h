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


namespace mar::editor {


	class ProjectSelectionFilesystem {
	public:

		ProjectSelectionFilesystem() = default;

		void openWindowNewProject();
		void windowNewProject();

		void openWindowOpenProject();
		void windowOpenProject();

	private:

		imgui_addons::ImGuiFileBrowser m_fileDialog;

		const char* m_nameNewProject;
		const char* m_nameOpenProject;

	};


}


#endif // !MAR_ENGINE_EDITOR_PROJECT_SELECTION_FILESYSTEM_H