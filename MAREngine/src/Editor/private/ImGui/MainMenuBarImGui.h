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


#ifndef MARENGINE_MAINMENUBARIMGUI_H
#define MARENGINE_MAINMENUBARIMGUI_H


#include <string>
#include "../../public/EditorWidget.h"


namespace marengine {

    class FFilesystemPopUpImGuiWidget;
    class FSceneManagerEditor;
    class FWindow;


    class FMainMenuBarWidgetImGui : public FMainMenuBarEditorWidget {
    public:

        void create(FServiceLocatorEditor* pServiceLocator) final;
        void updateFrame() override;

    private:

        void displaySceneManagementTab();
        void displayEntitiesManagementTab();
        void displaySettingsTab();
        void displayAboutTab();


        FFilesystemPopUpImGuiWidget* m_pFilesystem{ nullptr };
        FSceneManagerEditor* m_pSceneManagerEditor{ nullptr };
        FWindow* m_pWindow{ nullptr };

        bool m_infoAboutAuthorDisplay{ false };
        bool m_infoAboutEngineDisplay{ false };
        bool m_windowSettingsDisplay{ false };

        bool m_newSceneDisplay{ false };
        bool m_openSceneDisplay{ false };
        bool m_saveSceneDisplay{ false };

    };


}



#endif //MARENGINE_MAINMENUBARIMGUI_H
