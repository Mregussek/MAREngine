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


#ifndef MARENGINE_INSPECTOREDITORWIDGET_H
#define MARENGINE_INSPECTOREDITORWIDGET_H


#include "../../../mar.h"
#include "../../public/EditorWidget.h"


namespace marengine {

    class Entity;
    class FSceneManagerEditor;
    class FWindow;
    class FScriptWidgetImGui;
    class FFilesystemPopUpImGuiWidget;
    class FContentBrowserImGui;
    class FMaterialManager;


    class FInspectorWidgetImGui : public FInspectorEditorWidget {
    public:

        void create(FServiceLocatorEditor* pServiceLocator) final;

        void updateFrame() override;

    private:

        void displayPlayMode();
        void displayEditorMode();

        void popUpMenu() const;
        void displayChildrenPopMenu() const;
        void displayComponentPopMenu() const;

        template<typename TComponent>
        void handle(const char* componentName);

        template<typename TComponent>
        void displayComponentPanel();


        FScriptWidgetImGui* m_pScriptWidget{ nullptr };
        FSceneManagerEditor* m_pSceneManagerEditor{ nullptr };
        FMaterialManager* m_pMaterialManager{ nullptr };
        FFilesystemPopUpImGuiWidget* m_pFilesystem{ nullptr };
        FContentBrowserImGui* m_pContentBrowser{ nullptr };
        FWindow* m_pWindow{ nullptr };

        bool m_newScriptWindow{ false };
        bool m_assignScriptWindow{ false };
        bool m_loadTex2D{ false };

    };


}


#include "InspectorImGui.inl"


#endif //MARENGINE_INSPECTOREDITORWIDGET_H
