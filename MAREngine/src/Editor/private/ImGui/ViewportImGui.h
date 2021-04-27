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


#ifndef MARENGINE_VIEWPORTIMGUI_H
#define MARENGINE_VIEWPORTIMGUI_H


#include "../../public/EditorWidget.h"
#include "../../../Core/graphics/public/Camera.h"


namespace marengine {

    class FSceneManagerEditor;
    class FInspectorWidgetImGui;
    class FRenderManager;
    class FWindow;


    class FViewportWidgetImGui : public FViewportEditorWidget {
    public:

        void create(FServiceLocatorEditor* pServiceLocator) final;
        void updateFrame() override;

    private:

        MAR_NO_DISCARD ImGuizmo::OPERATION displayViewportControlPanel();
        void displayActualViewport();


        FCamera3D m_camera;
        float m_aspectRatio{ 1.33f };

        FSceneManagerEditor* m_pSceneManagerEditor{ nullptr };
        FInspectorWidgetImGui* m_pInspectorWidget{ nullptr };
        FRenderManager* m_pRenderManager{ nullptr };
        FWindow* m_pWindow{ nullptr };

    };


}



#endif //MARENGINE_VIEWPORTIMGUI_H