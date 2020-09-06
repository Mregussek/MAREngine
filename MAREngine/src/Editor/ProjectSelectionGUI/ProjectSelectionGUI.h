/**
 *           MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


#ifndef MAR_ENGINE_EDITOR_PROJECT_SELECTION_GUI_H
#define MAR_ENGINE_EDITOR_PROJECT_SELECTION_GUI_H


#include "../../mar.h"
#include "../../Window/Window.h"
#include "../GUI/GUI_Theme.h"


namespace mar {
	namespace editor {


		class ProjectSelectionGUI {
			static bool s_dockspaceOpen;
			static bool s_fullscreenPersisant;

		public:
			void initialize(const char* glsl_version) {
				ImGui::CreateContext();
				GUI_Theme::Setup_Theme();
				ImGui_ImplGlfw_InitForOpenGL(window::Window::getInstance().m_window.m_window, true);
				ImGui_ImplOpenGL3_Init(glsl_version);

				ImGuiIO& io = ImGui::GetIO();
				io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
				io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
				io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
			}

			void shutdown() {
				ImGui_ImplOpenGL3_Shutdown();
				ImGui_ImplGlfw_Shutdown();
				ImGui::DestroyContext();
			}

			void prepare() {
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();
				ImGuizmo::BeginFrame();

				static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

				ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
				if (s_fullscreenPersisant) {
					ImGuiViewport* viewport = ImGui::GetMainViewport();
					ImGui::SetNextWindowPos(viewport->GetWorkPos());
					ImGui::SetNextWindowSize(viewport->GetWorkSize());
					ImGui::SetNextWindowViewport(viewport->ID);
					ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
					ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
					window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
					window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
				}

				if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
					window_flags |= ImGuiWindowFlags_NoBackground;

				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
				ImGui::Begin("MAREngineProjectSelection", &s_dockspaceOpen, window_flags);
				ImGui::PopStyleVar();

				if (s_fullscreenPersisant)
					ImGui::PopStyleVar(2);

				ImGuiIO& io = ImGui::GetIO();
				if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
					ImGuiID dockspace_id = ImGui::GetID("MARProjectSelectionDockSpace");
					ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
				}
			}

			void update() {
				if (ImGui::BeginMainMenuBar()) {
					if (ImGui::BeginMenu("Menu One")) {


						ImGui::EndMenu();
					}

					ImGui::EndMainMenuBar();
				}

				ImGui::Begin("Window One");
				ImGui::End();
				ImGui::Begin("Window Two");
				ImGui::End();
			}

			void render() {
				ImGui::End();
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
				ImGui::EndFrame();
			}
		};


} }


#endif // !MAR_ENGINE_EDITOR_PROJECT_SELECTION_GUI_H
