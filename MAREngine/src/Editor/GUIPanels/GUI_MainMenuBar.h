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


#ifndef MAR_ENGINE_EDITOR_GUI_MAIN_MENU_BAR_H
#define MAR_ENGINE_EDITOR_GUI_MAIN_MENU_BAR_H


#include "../../mar.h"
#include "GUI_Filesystem.h"


namespace mar::ecs { class SceneManager; /* forward declaration */ }
namespace mar::editor {


	class GUI_MainMenuBar {
	public:

		GUI_MainMenuBar() = default;

		void setSceneManager(ecs::SceneManager* manager);
		
		void initialize();
		void display();

	private:

		void display_mainMenuBar();

		ecs::SceneManager* m_sceneManager{ nullptr };
		GUI_Filesystem m_guiFilesystem;

		bool m_newSceneWindow{ false };
		bool m_loadSceneWindow{ false };
		bool m_saveSceneWindow{ false };
		bool m_loadOBJfileWindow{ false };
		bool m_infoWindow{ false };
		bool m_instructionWindow{ false };

	};


}


#endif // !MAR_ENGINE_EDITOR_GUI_MAIN_MENU_BAR_H