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


#ifndef MARENGINE_DEBUGIMGUI_H
#define MARENGINE_DEBUGIMGUI_H


#include "../../public/EditorWidget.h"


namespace marengine {

    class FSceneManagerEditor;
    class Scene;
    class Entity;
    class FRenderStatistics;


    class FDebugWidgetImGui : public FDebugEditorWidget {
    public:

        void create(FServiceLocatorEditor* pServiceLocator) final;
        void updateFrame() override;

    private:

        void displayInfoAbout(Scene* pScene) const;
        void displayInfoAbout(const Entity& entity) const;

        FSceneManagerEditor* m_pSceneManagerEditor{ nullptr };
        FRenderStatistics* m_pRenderStatistics{ nullptr };

    };


}


#endif //MARENGINE_DEBUGIMGUI_H
