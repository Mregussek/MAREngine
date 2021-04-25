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


#include "MainImGui.h"
#include "../../public/ServiceLocatorEditor.h"
#include "../../../Core/ecs/SceneManagerEditor.h"
#include "../../../Window/Window.h"


namespace marengine {


    void FMainWidgetImGui::create(FServiceLocatorEditor* pServiceLocator) {
        m_pSceneManagerEditor =
                pServiceLocator->retrieve<FHolderPtr<FSceneManagerEditor*>>()->pInstance;
        m_pWindow =
                pServiceLocator->retrieve<FHolderPtr<FWindow*>>()->pInstance;

        ImGui::CreateContext();
        m_pWindow->initEditorGuiLibrary();
        ImGui_ImplOpenGL3_Init("#version 450");

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        m_pSceneManagerEditor->useEditorCamera();
    }

    void FMainWidgetImGui::destroy() {
        ImGui_ImplOpenGL3_Shutdown();
        m_pWindow->terminateEditorGuiLibrary();
        ImGui::DestroyContext();
    }

    void FMainWidgetImGui::beginFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        m_pWindow->beginNewFrameEditorGuiLibrary();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();

        constexpr ImGuiDockNodeFlags dockspaceFlags{ ImGuiDockNodeFlags_None };

        ImGuiWindowFlags windowFlags{ ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking };
        if (m_fullscreenPersistent) {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        if constexpr (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode) {
            windowFlags |= ImGuiWindowFlags_NoBackground;
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("MAREngineDockspace", &m_dockspaceOpen, windowFlags);
        ImGui::PopStyleVar();

        if (m_fullscreenPersistent) {
            ImGui::PopStyleVar(2);
        }

        const ImGuiIO& io{ ImGui::GetIO() };
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            const ImGuiID dockspace_id = ImGui::GetID("MARDockspace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
        }

        ImGui::ShowDemoWindow();
    }

    void FMainWidgetImGui::endFrame() {
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGui::EndFrame();
    }


}
