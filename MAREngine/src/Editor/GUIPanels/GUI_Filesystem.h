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


#ifndef MAR_ENGINE_EDITOR_GUI_FILESYSTEM_H
#define MAR_ENGINE_EDITOR_GUI_FILESYSTEM_H


#include "../../mar.h"


namespace marengine {

	class Scene; 
	class Entity; 
	struct ScriptComponent;


	class GUI_Filesystem {
	public:

		static GUI_Filesystem* Instance() { return s_instance; }

		void initialize();

		void openNewSceneWindow();
		void displayNewSceneWindow();

		void openSaveSceneWindow();
		void displaySaveSceneWindow(Scene* scene);

		void openLoadSceneWindow();
		void displayLoadSceneWindow();

		void openLoadOBJWindow();
		void displayLoadOBJWindow(Scene* scene);

		void openAssigningScriptWindow();
		void displayAssigningScriptWindow(ScriptComponent& script);

	private:

		void eraseSubstring(std::string& str, const std::string& toErase);

		imgui_addons::ImGuiFileBrowser m_fileDialog;

		static GUI_Filesystem* s_instance;

		const char* m_nameNewScene{ "New Scene" };
		const char* m_nameOpenScene{ "Open Scene" };
		const char* m_nameSaveScene{ "Save Scene" };
		const char* m_nameLoadOBJ{ "Load .obj file" };
		const char* m_nameAssignScript{ "Assign Script" };

	};


}


#endif // !MAR_ENGINE_EDITOR_GUI_FILESYSTEM_H
