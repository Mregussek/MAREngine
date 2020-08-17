/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

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

		private:

			void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace);

			void TextEditor_CreateNewScriptWindow();
			void TextEditor_OpenScriptWindow();
			void TextEditor_MenuBar();
			void TextEditor_Render();

			const TextEditor::LanguageDefinition& TextEditorLanguageDefinition_Python();
		};


} }