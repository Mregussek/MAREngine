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


#ifndef MARENGINE_VIEWPORTIMGUIWIDGET_H
#define MARENGINE_VIEWPORTIMGUIWIDGET_H


#include "../../IEditorWidget.h"
#include "../../../Camera/Camera.h"
#include "GuizmoImGuiWidget.h"
#include "../../../../Platform/OpenGL/FramebufferOpenGL.h"


namespace marengine {

    class FSceneManagerEditor;
    class FInspectorImGuiWidget;
    class FImGuiEditorServiceLocator;
    class FWindow;


    class FViewportImGuiWidget : public IViewportEditorWidget {
    public:

        void create(FImGuiEditorServiceLocator* serviceLocator);
        void destroy() override;

        void beginFrame() override;
        void updateFrame() override;

        void bind(maths::vec3 backgroundColor) const;

    private:

        void unbind() const;

        void updateAspectRatio();

        void displayViewportControlPanel();
        void displayActualViewport();
        void handleGuizmo();


        FramebufferOpenGL m_framebuffer;
        Camera m_camera;
        float m_aspectRatio{ 1.33f };
        FGuizmoImGuiWidget m_guizmo;

        FSceneManagerEditor* m_pSceneManagerEditor{ nullptr };
        FInspectorImGuiWidget* m_pInspectorWidget{ nullptr };
        FWindow* m_pWindow{ nullptr };

    };


}



#endif //MARENGINE_VIEWPORTIMGUIWIDGET_H
