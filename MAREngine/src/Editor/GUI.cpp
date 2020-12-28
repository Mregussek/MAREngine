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


#include "GUI.h"

#include "EditorLogging.h"
#include "Camera/Camera.h"

#include "../Core/ecs/Scene.h"
#include "../Core/ecs/SceneManager.h"

#include "../Window/Window.h"

#include "GUIPanels/OtherPanels/GUI_Theme.h"
#include "GUIPanels/OtherPanels/GUI_Statistics.h"


namespace marengine {


	void GUI::initialize(const char* glslVersion) {
		ImGui::CreateContext();
		GUI_Theme::Setup_Theme();

		Window::imguiInit();
		ImGui_ImplOpenGL3_Init(glslVersion);

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		SceneManager::Instance->useEditorCamera = true;

		m_viewport.initialize();
		m_textEditor.initialize();
		m_mainMenuBar.initialize();
		m_entityPanel.initialize();
		m_collectionPanel.initialize();

		EDITOR_INFO("GUI: initialized properly!");
	}

	void GUI::shutdown() {
		m_viewport.close();

		ImGui_ImplOpenGL3_Shutdown();

		Window::imguiTerminate();

		ImGui::DestroyContext();

		EDITOR_INFO("GUI: closed properly!");
	}

	void GUI::display() {
		prepareNewFrame();
		updateFrame();
		endFrame();
		
		EDITOR_INFO("GUI: displayed frame!");
	}

	void GUI::renderToViewport() const {
		m_viewport.bind(SceneManager::Instance->getScene()->getBackground());
	}

	const Entity& GUI::getCurrentEntity() const {
		return m_entityPanel.getCurrentEntity();
	}

	const EntityCollection& GUI::getCurrentCollection() const {
		return m_collectionPanel.getCurrentCollection();
	}

	bool GUI::canDrawLines() const {
		return SceneManager::Instance->isEditorMode() && SceneManager::Instance->useEditorCamera;
	}

	// --------- PRIVATE METHODS ------------- //

	void GUI::prepareNewFrame() {
		m_viewport.unbind();

		ImGui_ImplOpenGL3_NewFrame();

		Window::imguiNewFrame();
		
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();

		constexpr ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (m_fullscreenPersisant) {
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
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

		if (m_fullscreenPersisant) { ImGui::PopStyleVar(2); }
			
		const ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			const ImGuiID dockspace_id = ImGui::GetID("MARDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
		}

		EDITOR_TRACE("GUI: prepared frame (render, dockspace, other...)");
	}

	void GUI::updateFrame() {
		m_sceneHierarchyPanel.update();

		GUI_Statistics::update();

		m_textEditor.update();

		m_mainMenuBar.display();
		m_viewport.display(m_collectionPanel.getCurrentCollection(), m_entityPanel.getCurrentEntity());
		m_sceneProperties.display(SceneManager::Instance->getScene()->getBackground());

		m_entityPanel.update(SceneManager::Instance->isPlayMode());
		m_collectionPanel.update();

		EDITOR_TRACE("GUI: updated frame! (Actual Editor Windows)");
	}

	void GUI::endFrame() const {
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		ImGui::EndFrame();

		EDITOR_TRACE("GUI: ending frame! (rendering gathered data)");
	}


}
