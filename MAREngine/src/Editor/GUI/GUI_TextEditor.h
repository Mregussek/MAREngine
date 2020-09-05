/**
 *				MAREngine - open source 3D game engine
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
#include "../../Core/ecs/ECS/Systems.h"


namespace mar {
	namespace editor {


		class GUI_TextEditor {
			static GUI_TextEditor s_Instance;
			TextEditor editor;
			std::string m_title{ "Empty" };
			std::string m_pathToSave;
			bool m_createNewScriptWindow;
			bool m_openScriptWindow;

		public:
			static std::string DEFAULT_SCRIPT;

			GUI_TextEditor();

			static GUI_TextEditor& Instance() { return s_Instance; }

			void startup();
			void update();

			void createNewFile(std::string scriptPath);

			void setPathToSave(std::string s) { m_pathToSave = std::move(s); }
			void setEditorText(std::string s) { editor.SetText(std::move(s)); }
			void setEditorTitle(std::string new_title) { m_title = std::move(new_title); }

			void reset() {
				setEditorText("def main():\n\tpass\n");
				setEditorTitle("Empty");
			}

		private:

			void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace);

			void TextEditor_CreateNewScriptWindow();
			void TextEditor_OpenScriptWindow();
			void TextEditor_MenuBar();
			void TextEditor_Render();

			const TextEditor::LanguageDefinition& TextEditorLanguageDefinition_Python();
		};


} }

#endif // !MAR_ENGINE_EDITOR_GUI_TEXT_EDITOR_H