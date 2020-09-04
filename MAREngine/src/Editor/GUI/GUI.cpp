/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "GUI.h"
#include "../Filesystem/EditorFilesystem.h"
#include "../../Core/ecs/ECS/Entity.h"
#include "../../Core/ecs/ECS/ComponentsEntity.h"
#include "../../Core/ecs/ECS/Systems.h"
#include "../../Core/ecs/SceneEvents.h"
#include "../../Window/Input.h"
#include "GUI_TextEditor.h"
#include "GUI_SceneHierarchy.h"


namespace mar {
	namespace editor {


		bool GUI::s_dockspaceOpen{ true };
		bool GUI::s_fullscreenPersisant{ true };


		void GUI::initialize(const char* glsl_version) {
			ImGui::CreateContext();
			GUI_Theme::Setup_Theme();
			ImGui_ImplGlfw_InitForOpenGL(window::Window::getInstance().m_window.m_window, true);
			ImGui_ImplOpenGL3_Init(glsl_version);

			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
			io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

			m_viewportFramebuffer.initialize(platforms::FrameBufferSpecification(800.f, 600.f));

			GUI_TextEditor::Instance().setEditorText("def main():\n\tpass\n");

			EDITOR_INFO("GUI: initialized properly!");
		}

		void GUI::shutdown() {
			m_viewportFramebuffer.close();

			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();

			EDITOR_INFO("GUI: closed properly!");
		}

		void GUI::display() {
			prepareNewFrame();
			updateFrame();
			endFrame();

			EDITOR_INFO("GUI: displayed frame!");
		}

		void GUI::prepareNewFrame() {
			m_viewportFramebuffer.unbind();

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
			ImGui::Begin("MAREngineDockspace", &s_dockspaceOpen, window_flags);
			ImGui::PopStyleVar();

			if (s_fullscreenPersisant)
				ImGui::PopStyleVar(2);

			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
				ImGuiID dockspace_id = ImGui::GetID("MARDockspace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			EDITOR_TRACE("GUI: prepared frame (render, dockspace, other...)");
		}

		void GUI::updateFrame() {
			GUI_SceneHierarchy::Scene_Hierarchy(m_sceneManager);
			GUI_SceneHierarchy::Scene_Statistics();

			GUI_TextEditor::Instance().update();

			Editor_MainMenuBar();
			Editor_ViewPort();
			Editor_Properties();

			GUI_EntityPanel::Scene_Entity_Modify(m_sceneManager->isPlayMode());
			GUI_EntityCollectionPanel::Scene_EntityCollection_Modify();

			if (m_loadSceneWindow) {
				m_loadSceneWindow = GUI_Filesystem::Filesystem_LoadScene(m_loadSceneWindow);
			}
			if (m_saveSceneWindow) { 
				m_saveSceneWindow = GUI_Filesystem::Filesystem_SaveScene(m_saveSceneWindow, m_sceneManager->getScene());
			}
			if (m_infoWindow) { 
				GUI_Info::Menu_Info(m_infoWindow); 
			}
			if (m_instructionWindow) { 
				GUI_Info::Menu_Instruction(m_instructionWindow); 
			}

			EDITOR_TRACE("GUI: updated frame! (Actual Editor Windows)");
		}

		void GUI::endFrame() {
			ImGui::End();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			ImGui::EndFrame();

			EDITOR_TRACE("GUI: ending frame! (rendering gathered data)");
		}

		void GUI::Editor_MainMenuBar() {
			if (ImGui::BeginMainMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if(ImGui::MenuItem("New")) {
						GUI_EntityCollectionPanel::reset();
						GUI_EntityPanel::reset();
						GUI_TextEditor::Instance().reset();

						GUI_Filesystem::Filesystem_NewScene();
					}

					if (ImGui::MenuItem("Open")) {
						GUI_EntityCollectionPanel::reset();
						GUI_EntityPanel::reset();
						GUI_TextEditor::Instance().reset();

						m_loadSceneWindow = true;
					}

					if (ImGui::MenuItem("Save")) {
						m_saveSceneWindow = true;
					}

					if (ImGui::MenuItem("Exit")) {
						window::Window::getInstance().endRenderLoop();
					}

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("About")) {
					m_infoWindow = true;
					m_instructionWindow = true;
				}

				if (ImGui::MenuItem("Exit")) {
					window::Window::getInstance().endRenderLoop();
				}

				ImGui::EndMainMenuBar();
			}

			EDITOR_TRACE("GUI: pushing main menu bar");
		}

		void GUI::Editor_ViewPort() {
			ImGui::Begin("ViewPort", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

			if (ImGui::IsWindowFocused()) {
				m_mouseViewportX = window::Input::getMousePositionX();
				m_mouseViewportY = window::Input::getMousePositionY();
				m_enableViewportInput = true;
			}
			else {
				m_mouseViewportX = 0.f;
				m_mouseViewportY = 0.f;
				m_enableViewportInput = false;
			}

			if (ImGui::BeginMenuBar()) {
				if (m_sceneManager->isEditorMode()) {
					if (ImGui::Button("PLAY")) {
						m_sceneManager->setPlayMode();
					}
				}
				else {
					if (ImGui::Button("STOP")) {
						m_sceneManager->setExitPlayMode();
					}

					ImGui::SameLine();

					if (!m_sceneManager->isPauseMode()) {
						if (ImGui::Button("PAUSE"))
							m_sceneManager->setPauseMode();
					}
					else {
						if (ImGui::Button("RESUME"))
							m_sceneManager->unsetPauseMode();
					}
				}

				ImGui::EndMenuBar();
			}

			static uint32_t id;

			auto& spec = m_viewportFramebuffer.getSpecification();
			id = m_viewportFramebuffer.getColorAttach();

			ImVec2 size = ImGui::GetContentRegionAvail();
			spec.width = size.x;
			spec.height = size.y;

			ImGui::Image((void*)id, ImVec2{ spec.width, spec.height }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			ImGui::End();

			EDITOR_TRACE("GUI: Displaying viewport");
		}

		void GUI::Editor_Properties() {
			ImGui::Begin("Editor Properties");

			ImGui::Checkbox("UseCameraEditor", &m_sceneManager->useEditorCamera);

			auto& scene_background = m_sceneManager->getScene()->getBackground();

			if (ImGui::ColorEdit3("Scene Background Color", maths::vec3::value_ptr_nonconst(scene_background))) {
				auto& spec = m_viewportFramebuffer.getSpecification();
				spec.backgroundColor = scene_background;
				window::Window::getInstance().updateBackgroundColor(scene_background);
			}

			ImGui::End();
		}

		void GUI::submit(ecs::SceneManager* scene) {
			m_sceneManager = scene;
			m_sceneManager->useEditorCamera = true;
			GUI_EntityPanel::render_cam = &m_sceneManager->getScene()->getRenderCamera();

			EDITOR_INFO("GUI: scene has been submitted!");
		}

		ecs::Entity* GUI::getCurrentEntity() {
			return GUI_EntityPanel::currentEntity;
		}


} }