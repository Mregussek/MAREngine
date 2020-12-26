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


#ifndef MAR_ENGINE_EDITOR_GUI_TEXT_EDITOR_H
#define MAR_ENGINE_EDITOR_GUI_TEXT_EDITOR_H


#include "../../mar.h"
#include "../../Core/ecs/Entity/Entity.h"


namespace marengine {


	class GUI_TextEditor {
	public:
		
		// --- public methods --- //

		GUI_TextEditor() = default;

		static GUI_TextEditor* Instance() { return s_instance; }

		void initialize();
		void update();
		void reset();

		void setPathToSave(std::string s);
		void setEditorText(std::string s);
		void setEditorTitle(std::string new_title);

		void setCreatingNewScript() { m_createNewScriptWindow = true;  }
		void setLoadingScript() { m_openScriptWindow = true; }

	private:

		// --- private methods --- //

		std::string replaceOcurrences(std::string str, const std::string& from, const std::string& to);
		
		void createNewScriptWindow();
		void openScriptWindow();
		void displayMainMenuBar();
		void editorRender();

		void createNewFile(const std::string& scriptPath, const std::string& moduleName);
		void openFile(const std::string& scriptPath, const std::string& moduleName);

		void definePythonLanguage();

		// --- members --- //

		static GUI_TextEditor* s_instance;

		TextEditor::LanguageDefinition m_languageDefinition;

		TextEditor editor;

		std::string m_title{ defaultTitle };
		std::string m_pathToSave;

		bool m_createNewScriptWindow{ false };
		bool m_openScriptWindow{ false };

		static const std::string defaultScript;
		static const std::string defaultTitle;

	};


}

#endif // !MAR_ENGINE_EDITOR_GUI_TEXT_EDITOR_H