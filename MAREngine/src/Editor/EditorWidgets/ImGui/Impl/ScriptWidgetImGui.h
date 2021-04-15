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


#ifndef MARENGINE_SCRIPTWIDGETIMGUI_H
#define MARENGINE_SCRIPTWIDGETIMGUI_H


#include "../../IEditorWidget.h"
#include "../../../../mar.h"


namespace marengine {

    class FImGuiEditorServiceLocator;


    class FScriptWidgetImGui : public FScriptEditorWidget {
    public:

        void create(FImGuiEditorServiceLocator* serviceLocator);
        void reset();

        void updateFrame() override;

        void setEditorTitle(std::string newTitle);
        void setEditorCode(const std::string& sourceCode);
        void setPathToScript(std::string pathToScript);

        std::string getEditorSourceCode() const;
        static std::string getDefaultEditorSourceCode() ;
        static std::string getDefaultEditorTitle() ;

        bool isEditorCurrentlyUsed() const;

    private:

        void displayMainMenuBar();
        void editorRender();


        TextEditor::LanguageDefinition m_languageDefinition;
        TextEditor editor;

        std::string m_title;
        std::string m_pathToScript;

    };


}




#endif //MARENGINE_SCRIPTWIDGETIMGUI_H
