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
#include "../Core/ecs/SceneEvents.h"

#include "../Window/Window.h"

#include "GUIPanels/OtherPanels/GUI_Theme.h"
#include "GUIPanels/OtherPanels/GUI_Statistics.h"


namespace mar::editor {


	void GUI::initialize(const char* glslVersion) {
		ImGui::CreateContext();
		GUI_Theme::Setup_Theme();

		window::Window::imguiInit();
		ImGui_ImplOpenGL3_Init(glslVersion);

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		constexpr float xDefault = 800.f;
		constexpr float yDefault = 600.f;
		m_viewportFramebuffer.initialize(xDefault, yDefault);
		updateViewportAspectRatio();

		m_textEditor.initialize();

		m_mainMenuBar.initialize();
		m_entityPanel.initialize();
		m_collectionPanel.initialize();

		ecs::SceneEvents::Instance().onEditorCameraSet(&Camera::getCameraData());

		EDITOR_INFO("GUI: initialized properly!");
	}

	void GUI::shutdown() {
		m_viewportFramebuffer.close();

		ImGui_ImplOpenGL3_Shutdown();

		window::Window::imguiTerminate();

		ImGui::DestroyContext();

		EDITOR_INFO("GUI: closed properly!");
	}

	void GUI::submit(ecs::SceneManager* scene) {
		m_sceneManager = scene;
		m_sceneManager->useEditorCamera = true;
		m_mainMenuBar.setSceneManager(m_sceneManager);

		EDITOR_INFO("GUI: scene has been submitted!");
	}

	void GUI::bind() const {
		m_viewportFramebuffer.bind();
		m_viewportFramebuffer.clear(m_sceneManager->getScene()->getBackground());
	}

	void GUI::display() {
		prepareNewFrame();
		updateFrame();
		endFrame();
		
		EDITOR_INFO("GUI: displayed frame!");
	}

	void GUI::updateViewportAspectRatio() {
		const auto size = m_viewportFramebuffer.getSize();
		m_viewportAspectRatio = size.x / size.y;
	}

	float GUI::getViewportAspectRatio() const {
		return m_viewportAspectRatio;
	}

	platforms::FramebufferOpenGL& GUI::getFramebuffer() {
		return m_viewportFramebuffer;
	}

	const ecs::Entity& GUI::getCurrentEntity() const {
		return m_entityPanel.getCurrentEntity();
	}

	const ecs::EntityCollection& GUI::getCurrentCollection() const {
		return m_collectionPanel.getCurrentCollection();
	}

	bool GUI::canDrawLines() const {
		return m_sceneManager->isEditorMode() && m_sceneManager->useEditorCamera;
	}

	// --------- PRIVATE METHODS ------------- //

	void GUI::prepareNewFrame() {
		m_viewportFramebuffer.unbind();

		ImGui_ImplOpenGL3_NewFrame();

		window::Window::imguiNewFrame();
		
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
		m_sceneHierarchyPanel.update(m_sceneManager);

		GUI_Statistics::update(m_sceneManager);

		m_textEditor.update();

		m_mainMenuBar.display();
		Editor_ViewPort();
		Editor_Properties();

		m_entityPanel.update(m_sceneManager->isPlayMode());
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

	void GUI::Editor_ViewPort() {
		ImGui::Begin("ViewPort", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar()) {
			if (m_sceneManager->isEditorMode()) {
				if (ImGui::Button("PLAY")) { m_sceneManager->setPlayMode(); }
			}
			else {
				if (ImGui::Button("STOP")) { 
					m_sceneManager->setExitPlayMode(); 
					if (m_sceneManager->useEditorCamera) {
						ecs::SceneEvents::Instance().onEditorCameraSet(&Camera::getCameraData());
					}
				}

				ImGui::SameLine();

				if (!m_sceneManager->isPauseMode()) {
					if (ImGui::Button("PAUSE")) { m_sceneManager->setPauseMode(); }
				}
				else {
					if (ImGui::Button("RESUME")) { m_sceneManager->unsetPauseMode(); }
				}
			}

			ImGui::EndMenuBar();
		}

		const ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		const auto size = m_viewportFramebuffer.getSize();

		if (size.x != viewportSize.x || size.y != viewportSize.y) {
			m_viewportFramebuffer.resize(viewportSize.x, viewportSize.y);
			updateViewportAspectRatio();
		}

		const uint32_t id = m_viewportFramebuffer.getColorAttach();
		ImGui::Image((ImTextureID)id, viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();

		EDITOR_TRACE("GUI: Displaying viewport");
	}

	void GUI::Editor_Properties() {
		ImGui::Begin("Editor Properties");

		if (ImGui::Checkbox("UseCameraEditor", &m_sceneManager->useEditorCamera)) {
			if (m_sceneManager->isEditorMode()) {
				if (m_sceneManager->useEditorCamera) { 
					ecs::SceneEvents::Instance().onEditorCameraSet(&Camera::getCameraData()); 
				}
				else { 
					ecs::SceneEvents::Instance().onGameCameraSet(); 
				}
			}
		}

		auto& sceneBackground = m_sceneManager->getScene()->getBackground();

		ImGui::ColorEdit3("Scene Background Color", &sceneBackground.x);

		ImGui::End();
	}


}
