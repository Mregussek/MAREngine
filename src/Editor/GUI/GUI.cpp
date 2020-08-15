/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "GUI.h"
#include "../Filesystem/EditorFilesystem.h"
#include "../../Core/ecs/ECS/Entity.h"
#include "../../Core/ecs/ECS/Components.h"
#include "../../Core/ecs/ECS/Systems.h"
#include "../../Core/ecs/SceneEvents.h"


namespace mar {
	namespace editor {


		bool GUI::s_dockspaceOpen{ true };
		bool GUI::s_fullscreenPersisant{ true };


		void GUI::initialize(const char* glsl_version) {
			ImGui::CreateContext();
			GUI_Theme::Setup_Theme();
			ImGui_ImplGlfw_InitForOpenGL(window::Window::getInstance().getWindow(), true);
			ImGui_ImplOpenGL3_Init(glsl_version);

			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
			io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

			m_viewportFramebuffer.initialize(graphics::FrameBufferSpecification(800.f, 600.f));

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
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGuizmo::BeginFrame();

			m_viewportFramebuffer.unbind();

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
			Menu_MainMenuBar();

			Scene_Properties();
			Scene_Hierarchy();
			Scene_Statistics();

			GUI_EntityManagement::Scene_Entity_Modify(m_sceneManager->isPlayMode());

			if (m_loadSceneWindow) { 
				GUI_Filesystem::Filesystem_LoadScene(m_loadSceneWindow); 
			}
			if (m_saveSceneWindow) { 
				GUI_Filesystem::Filesystem_SaveScene(m_saveSceneWindow, m_sceneManager->getScene()); 
			}
			if (m_infoWindow) { 
				GUI_Info::Menu_Info(m_infoWindow); 
			}
			if (m_instructionWindow) { 
				GUI_Info::Menu_Instruction(m_instructionWindow); 
			}

			Display_ViewPort();

			EDITOR_TRACE("GUI: updated frame! (Actual Editor Windows)");
		}

		void GUI::endFrame() {
			ImGui::End();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			ImGui::EndFrame();

			EDITOR_TRACE("GUI: ending frame! (rendering gathered data)");
		}

		void GUI::Menu_MainMenuBar() {
			if (ImGui::BeginMainMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("Open")) {
						m_loadSceneWindow = true;
					}

					if (ImGui::MenuItem("Save")) {
						m_saveSceneWindow = true;
					}

					if (ImGui::MenuItem("Exit")) {
						window::Window::getInstance().closeWindow();
					}

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("About")) {
					m_infoWindow = true;
					m_instructionWindow = true;
				}

				if (ImGui::MenuItem("Exit")) {
					window::Window::getInstance().closeWindow();
				}

				ImGui::EndMainMenuBar();
			}

			EDITOR_TRACE("GUI: pushing main menu bar");
		}

		void GUI::Display_ViewPort() {
			ImGui::Begin("ViewPort");

			if (ImGui::IsWindowFocused()) window::Input::enableInput();
			else window::Input::disableInput();

			static uint32_t id;

			auto& spec = m_viewportFramebuffer.getSpecification();
			id = m_viewportFramebuffer.getColorAttach();

			ImVec2 size = ImGui::GetContentRegionAvail();
			spec.width = size.x;
			spec.height = size.y;

			ImGui::Image((void*)id, ImVec2{ spec.width, spec.height }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			ImGui::End();

			ImGui::Begin("Manage Viewport");

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

			ImGui::End();

			EDITOR_TRACE("GUI: Displaying viewport");
		}

		void GUI::Scene_Statistics() {
			ImGui::Begin("Statistics Menu");

			using stats = graphics::RendererEntity;

			auto drawcalls = "Draw Calls: " + std::to_string(stats::getStatistics().drawCallsCount);
			auto shapescount = "Shapes Count: " + std::to_string(stats::getStatistics().shapesCount);
			auto vertices = "Vertices: " + std::to_string(stats::getStatistics().verticesCount);
			auto indices = "Indices: " + std::to_string(stats::getStatistics().indicesCount);
			auto triangles = "Triangles: " + std::to_string(stats::getStatistics().trianglesCount);

			ImGui::Text(drawcalls.c_str());
			ImGui::Text(shapescount.c_str());
			ImGui::Text(vertices.c_str());
			ImGui::Text(indices.c_str());
			ImGui::Text(triangles.c_str());

			ImGui::Separator();

			static double lasttime = GetTickCount() * 0.001;
			static double currenttime;
			static double fps = 0.0;
			static int frames = 0;

			currenttime = GetTickCount() * 0.001;
			frames++;

			if (currenttime - lasttime > 1.0) {
				fps = frames / (currenttime - lasttime);
				frames = 0;
				lasttime = currenttime;
			}

			ImGui::Text("My FPS: %f ms/frame", fps);

			ImGui::Separator();

			std::string fpsinfo = "FPS: " + std::to_string(ImGui::GetIO().Framerate);
			std::string msframe = "ms/frame: " + std::to_string(1000.0f / ImGui::GetIO().Framerate);

			ImGui::Text(fpsinfo.c_str());
			ImGui::Text(msframe.c_str());

			ImGui::End();

			EDITOR_TRACE("GUI: scene_statistics");
		}

		void GUI::submit(ecs::SceneManager* scene) {
			m_sceneManager = scene;
			m_sceneManager->useEditorCamera = true;

			EDITOR_INFO("GUI: scene has been submitted!");
		}

		void GUI::Scene_Properties() {
			ImGui::Begin("Scene Properties");

			ImGui::Checkbox("UseCameraEditor", &m_sceneManager->useEditorCamera);

			ImGui::End();
		}

		void GUI::Scene_Hierarchy() {
			ImGui::Begin("Scene Hierarchy");

			ImGui::Text(" - ");
			ImGui::SameLine();
			ImGui::Text(m_sceneManager->getScene()->getName().c_str());

			auto& entities = m_sceneManager->getScene()->getEntities();

			for (int32_t i = 0; i < (int32_t)entities.size(); i++) {
				std::string& s = entities[i].getComponent<ecs::TagComponent>();
				if (ImGui::MenuItem(s.c_str())) {
					m_indexEntity = i;
					m_currentEntity = &m_sceneManager->getScene()->getEntity(m_indexEntity);
					GUI_EntityManagement::currentEntity = m_currentEntity;
					GUI_EntityManagement::currentIndex = m_indexEntity;
				}
			}

			Scene_Hierarchy_PopUp();

			ImGui::End();

			EDITOR_TRACE("GUI: scene_hierarchy");
		}

		void GUI::Scene_Hierarchy_PopUp() {
			if (m_sceneManager->isPlayMode()) {
				EDITOR_TRACE("GUI: return from scene_hierarchy_popup (PLAY MODE)");
				
				return;
			}

			static bool b = false;

			if (ImGui::IsWindowFocused())
				b = window::Input::isMousePressed_NotViewport(MAR_MOUSE_BUTTON_2);
			else
				b = false;

			if (b) {
				ImGui::OpenPopup("SceneHierarchyPopUp");
				if (window::Input::isMousePressed_NotViewport(MAR_MOUSE_BUTTON_1))
					b = false;
			}

			if (ImGui::BeginPopup("SceneHierarchyPopUp")) {
				if (ImGui::MenuItem("Add Entity to scene")) {
					m_currentEntity = &m_sceneManager->getScene()->createEntity();
					m_currentEntity->addComponent<ecs::TransformComponent>(ECS_TRANSFORM);
					m_indexEntity = m_sceneManager->getScene()->getEntities().size() - 1;
					GUI_EntityManagement::currentEntity = m_currentEntity;
					GUI_EntityManagement::currentIndex = m_indexEntity;
				}

				if(m_currentEntity)
					if (ImGui::MenuItem("Delete Selected Entity from Scene")) {
						m_sceneManager->getScene()->destroyEntity(m_indexEntity);
						m_indexEntity = -1;
						m_currentEntity = nullptr;
						GUI_EntityManagement::currentEntity = nullptr;
						GUI_EntityManagement::currentIndex = -1;
						ecs::SceneEvents::Instance().onEntityRemove();
					}

				ImGui::EndPopup();
			}

			EDITOR_TRACE("GUI: scene_hierarchy_popup");
		}
		
		


} }