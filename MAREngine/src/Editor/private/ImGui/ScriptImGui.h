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


#ifndef MARENGINE_SCRIPTIMGUI_H
#define MARENGINE_SCRIPTIMGUI_H


#include "../../public/EditorWidget.h"
#include "../../../mar.h"


namespace marengine {

    class FServiceLocatorEditor;


    class FScriptWidgetImGui : public FScriptEditorWidget {
    public:

        void create(FServiceLocatorEditor* pServiceLocator) final;
        void reset() final;

        void updateFrame() override;

        void setEditorTitle(std::string newTitle) final;
        void setEditorCode(const std::string& sourceCode) final;
        void setPathToScript(std::string pathToScript) final;

        std::string getEditorSourceCode() const final;
        std::string getDefaultEditorSourceCode() const final;
        std::string getDefaultEditorTitle() const final;

        bool isEditorCurrentlyUsed() const final;

    private:

        void displayMainMenuBar();
        void editorRender();


        TextEditor::LanguageDefinition m_languageDefinition;
        TextEditor m_editor;

        std::string m_title;
        std::string m_pathToScript;

    };


}




#endif //MARENGINE_SCRIPTIMGUI_H
