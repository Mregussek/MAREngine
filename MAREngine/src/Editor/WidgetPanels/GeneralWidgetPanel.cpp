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


#include "GeneralWidgetPanel.h"
#include "../EditorLogging.h"
#include "../Camera/Camera.h"
#include "../../Core/ecs/Scene.h"
#include "../../Core/ecs/SceneManagerEditor.h"
#include "../../Window/Window.h"


namespace marengine {


	void WGeneralWidgetPanel::create() {
		ImGui::CreateContext();
		Window::imguiInit();
		ImGui_ImplOpenGL3_Init("#version 450");

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		FSceneManagerEditor::Instance->useEditorCamera();

		EDITOR_INFO("GUI: initialized properly!");
	}

	void WGeneralWidgetPanel::destroy() {
		ImGui_ImplOpenGL3_Shutdown();
		Window::imguiTerminate();
		ImGui::DestroyContext();

		EDITOR_INFO("GUI: closed properly!");
	}

	void WGeneralWidgetPanel::beginFrame() {
		ImGui_ImplOpenGL3_NewFrame();

		Window::imguiNewFrame();
		
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();

		constexpr ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (s_fullscreenPersisant) {
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode) { windowFlags |= ImGuiWindowFlags_NoBackground; }

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("MAREngineDockspace", &m_dockspaceOpen, windowFlags);
		ImGui::PopStyleVar();

		if (s_fullscreenPersisant) { ImGui::PopStyleVar(2); }
			
		const ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			const ImGuiID dockspace_id = ImGui::GetID("MARDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
		}

		EDITOR_TRACE("GUI: prepared frame (render, dockspace, other...)");

		ImGui::ShowDemoWindow();
	}

	void WGeneralWidgetPanel::endFrame() {
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		ImGui::EndFrame();

		EDITOR_TRACE("GUI: ending frame! (rendering gathered data)");
	}


}
