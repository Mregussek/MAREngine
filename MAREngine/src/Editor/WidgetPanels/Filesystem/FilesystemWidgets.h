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


#ifndef MAR_ENGINE_W_FILESYSTEM_WIDGETS_H
#define MAR_ENGINE_W_FILESYSTEM_WIDGETS_H


#include "../../../mar.h"
#include "../IWidgetPanel.h"


namespace marengine {

	class Scene; 
	struct ScriptComponent;


	class WFilesystemWidgets : public IWidgetPanel {
	public:

		static WFilesystemWidgets* Instance;

		virtual void create() override;
		virtual void updateFrame() override;

		void openNewSceneWindow() const;
		void openSaveSceneWindow() const;
		void openLoadSceneWindow() const;
		void openLoadOBJWindow() const;
		void openAssigningScriptWindow() const;

	private:

		void displayNewSceneWindow();
		void displaySaveSceneWindow(Scene* scene);
		void displayLoadSceneWindow();
		void displayLoadOBJWindow(Scene* scene);
		void displayAssigningScriptWindow(ScriptComponent& script);

		void eraseSubstring(std::string& str, const std::string& toErase);


		imgui_addons::ImGuiFileBrowser m_fileDialog;

		const char* m_nameNewScene{ "New Scene" };
		const char* m_nameOpenScene{ "Open Scene" };
		const char* m_nameSaveScene{ "Save Scene" };
		const char* m_nameLoadOBJ{ "Load .obj file" };
		const char* m_nameAssignScript{ "Assign Script" };

	};


}


#endif // !MAR_ENGINE_W_FILESYSTEM_WIDGETS_H
