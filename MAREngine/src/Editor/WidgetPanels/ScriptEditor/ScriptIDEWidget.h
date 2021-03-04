/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#ifndef MAR_ENGINE_W_SCRIPT_IDE_H
#define MAR_ENGINE_W_SCRIPT_IDE_H


#include "../../../mar.h"
#include "../IWidgetPanel.h"


namespace marengine {


	class WScriptIDE : public IWidgetPanel {
	public:
		
		static WScriptIDE* Instance;

		virtual void create() override;
		virtual void updateFrame() override;

		void reset();

		void setEditorTitle(std::string newTitle);
		void setEditorCode(std::string sourceCode);
		void setPathToScript(std::string pathToScript);

		std::string getEditorSourceCode() const;
		static const std::string& getDefaultEditorSourceCode();

		bool isEditorCurrentlyUsed() const;

	private:

		std::string replaceOcurrences(std::string str, const std::string& from, const std::string& to);
		
		void displayMainMenuBar();
		void editorRender();

		void definePythonLanguage();


		TextEditor::LanguageDefinition m_languageDefinition;

		TextEditor editor;

		std::string m_title{ s_defaultTitle };
		std::string m_pathToScript{ "" };

		static const std::string s_titleNull;
		static const std::string s_editorTextNull;

		static const std::string s_defaultScript;
		static const std::string s_defaultTitle;

	};


}

#endif // !MAR_ENGINE_W_SCRIPT_IDE_H