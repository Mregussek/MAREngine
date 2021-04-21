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


#include "EnvironmentPropertiesWidgetImGui.h"
#include "../ImGuiEditorServiceLocator.h"
#include "../../../../Core/ecs/SceneManagerEditor.h"
#include "../../../../Core/ecs/Scene.h"
#include "../../../../Core/graphics/public/RenderManager.h"
#include "../../../../Core/graphics/public/Framebuffer.h"


namespace marengine {


    void FEnvironmentPropertiesWidgetImGui::create(FImGuiEditorServiceLocator* serviceLocator) {
        m_pRenderManager =
                serviceLocator->retrieve<FImGuiTypeHolder<FRenderManager*>>()->pInstance;
        m_pSceneManagerEditor =
                serviceLocator->retrieve<FImGuiTypeHolder<FSceneManagerEditor*>>()->pInstance;
    }

    void FEnvironmentPropertiesWidgetImGui::updateFrame() {
        ImGui::Begin("Environment Properties");

        static maths::vec3 clearColor{ 0.25f, 0.6f, 0.8f };
        if(ImGui::ColorEdit3("Scene Background Color", &clearColor.x)) {
            FFramebuffer* pFramebuffer{ m_pRenderManager->getViewportFramebuffer() };
            pFramebuffer->setClearColor(clearColor);
            m_pSceneManagerEditor->getScene()->setBackground(clearColor);
        }

        ImGui::End();
    }


}
