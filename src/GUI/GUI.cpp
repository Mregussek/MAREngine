/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "GUI.h"
#include "../Debug/Log.h"

namespace mar {
	namespace gui {

		const const char* GUITextureList::s_textures[] = {
			"empty",
			graphics::TexturePaths.mrTex.c_str(),
			graphics::TexturePaths.redTex.c_str(),
			graphics::TexturePaths.blueTex.c_str(),
			graphics::TexturePaths.blackTex.c_str(),
			graphics::TexturePaths.yellowTex.c_str(),
			graphics::TexturePaths.wallTex.c_str(),
			graphics::TexturePaths.grassTex.c_str()
		};
		int GUITextureList::s_selectedItem;

		GUIData GUI::s_guiData;
		bool GUI::s_dockspaceOpen{ true };
		bool GUI::s_fullscreenPersisant{ true };

		void GUI::ShowExampleAppDockSpace() {
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
			ImGui::Begin("DockSpace Demo", &s_dockspaceOpen, window_flags);
			ImGui::PopStyleVar();

			if (s_fullscreenPersisant)
				ImGui::PopStyleVar(2);

			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMenuBar()) {
				if (ImGui::BeginMenu("Docking")) {
					if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
					if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
					if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
					if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))     dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
					if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
					ImGui::Separator();
					if (ImGui::MenuItem("Close DockSpace", NULL, false))
						s_dockspaceOpen = false;
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			Menu_Info();
			Menu_ModifyScene();

			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}



		void GUI::initialize(window::Window* window, const char* glsl_version, bool can_modify_objects) {
			m_window = window;
			m_canModifyObjects = can_modify_objects;

			ImGui::CreateContext();
			ImGui::StyleColorsDark();
			ImGui_ImplGlfw_InitForOpenGL(m_window->getWindow(), true);
			ImGui_ImplOpenGL3_Init(glsl_version);

			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

			for (auto& c : m_sceneColors) c = 1.0f;

			MAR_CORE_INFO("GUI has initialized properly!");
		}

		void GUI::shutdown() {
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();

			MAR_CORE_INFO("GUI is closed!");
		}

		void GUI::prepareNewFrame() {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			s_guiData.translate = getTranslationMatrix();
			s_guiData.rotation = getRotationMatrix();
			s_guiData.colors[0] = m_sceneColors[0];
			s_guiData.colors[1] = m_sceneColors[1];
			s_guiData.colors[2] = m_sceneColors[2];
			s_guiData.colors[3] = m_sceneColors[3];
		}

		void GUI::display() {
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
			ImGui::Begin("DockSpace Demo", &s_dockspaceOpen, window_flags);
			ImGui::PopStyleVar();

			if (s_fullscreenPersisant)
				ImGui::PopStyleVar(2);

			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMainMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("New")) {

					}

					if (ImGui::MenuItem("Open")) {

					}

					if (ImGui::MenuItem("Save")) {

					}

					if (ImGui::MenuItem("Exit")) {
						glfwSetWindowShouldClose(m_window->getWindow(), true);
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Scene")) {
					if (ImGui::BeginMenu("Add Shape to Scene Window")) {
						Menu_PushShapeToScene();
						ImGui::EndMenu();
					}

					if (ImGui::MenuItem("Open Window to modify scene and meshes", nullptr, &m_modifySceneWindowDisplay)) {
						m_modifySceneWindowDisplay = true;
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Scene Statistics")) {
					Menu_Statistics();
					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("About")) {
					m_infoWindow = true;
				}

				if (ImGui::BeginMenu("Exit")) {
					glfwSetWindowShouldClose(m_window->getWindow(), true);
					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}

			if (m_modifySceneWindowDisplay) { Menu_ModifyScene(); }
			if (m_infoWindow) { Menu_Info(); }

			ImGui::End();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			ImGui::EndFrame();
		}

		void GUI::Menu_ModifyScene() {
			ImGui::Begin("Scene Modification Menu");

			if (m_canModifyObjects) {
				ImGui::MenuItem("Scene Menu");

				if (ImGui::TreeNode("Modify Scene")) {
					ImGui::SliderFloat3("T", &m_sceneTranslation.x, -5.0f, 5.0f);
					ImGui::SliderFloat3("R", &m_sceneAngle.x, -360.0f, 360.0f);
					ImGui::ColorEdit4("C", m_sceneColors);

					ImGui::TreePop();
				}

				ImGui::MenuItem("Shapes Menu", "");

				for (unsigned int index = 0; index < m_meshes.size(); index++) {

					std::string mesh = "M" + std::to_string(index);

					if (ImGui::TreeNode(("Modify " + mesh).c_str())) {
						for (unsigned int i = 0; i < m_meshes[index]->getShapesCount(); i++) {
							std::string shapeindex = mesh + m_meshes[index]->getName(i)[0] + std::to_string(i);
							std::string shapetrans = "T" + shapeindex;
							std::string shaperot = "R" + shapeindex;

							ImGui::Text(shapeindex.c_str());
							ImGui::SliderFloat3(shapetrans.c_str(), &m_meshes[index]->getCenter(i).x, -15.0f, 15.0f, "%.2f", 1.f);
							ImGui::SliderFloat3(shaperot.c_str(), &m_meshes[index]->getAngle(i).x, 0.0f, 360.0f, "%.2f", 1.f);
							
							std::string delete_shape = "Delete " + shapeindex;

							if (ImGui::Button(delete_shape.c_str())) {
								m_meshes[index]->flushShape(i);
							}
						}

						ImGui::TreePop();
					}

					ImGui::Separator();

				}
			}
			else {
				ImGui::Text("You cannot modify objects!");
			}

			ImGui::Separator();

			if (ImGui::Button("Close")) {
				m_modifySceneWindowDisplay = false;
			}

			ImGui::End();
		}

		void GUI::Menu_PushShapeToScene() {
			if (m_canModifyObjects) {
				ImGui::Text("Give value for each coordinate, which is in range (-10, 10)");
				ImGui::InputFloat3("Input Center", m_inputCenter);

				if (m_inputCenter[0] > 10.0f || m_inputCenter[0] < -10.0f)
					return;
				else if (m_inputCenter[1] > 10.0f || m_inputCenter[1] < -10.0f)
					return;
				else if (m_inputCenter[2] > 10.0f || m_inputCenter[2] < -10.0f)
					return;

				if (m_meshes[0]->getShapesCount() == graphics::constants::maxObjectsInScene - 1)
					return;

				ImGui::Combo("Choose Texture", &GUITextureList::s_selectedItem, GUITextureList::s_textures, IM_ARRAYSIZE(GUITextureList::s_textures));

				ImGui::Separator();

				if (ImGui::Button("Select Pyramid")) {
					Ref<graphics::Shape> new_shape = graphics::MeshCreator::createPyramid();
					glm::vec3 center{ m_inputCenter[0], m_inputCenter[1] , m_inputCenter[2] };
					glm::vec3 angle{ 0.0f, 0.0f, 0.0f };
					std::string texture = GUITextureList::s_textures[GUITextureList::s_selectedItem];

					m_meshes[0]->tryReuseShape(new_shape, center, angle, texture);
				}

				if (ImGui::Button("Select Cube")) {
					Ref<graphics::Shape> new_shape = graphics::MeshCreator::createCube();
					glm::vec3 center{ m_inputCenter[0], m_inputCenter[1] , m_inputCenter[2] };
					glm::vec3 angle{ 0.0f, 0.0f, 0.0f };
					std::string texture = GUITextureList::s_textures[GUITextureList::s_selectedItem];

					if (texture == "empty") {
						m_meshes[0]->tryReuseShape(new_shape, center, angle, texture);
						return;
					}
						
					std::vector<std::string> faces = { texture, texture, texture, texture, texture, texture };

					m_meshes[1]->tryReuseShape(new_shape, center, angle, faces);
				}

				if (ImGui::Button("Select Surface")) {
					Ref<graphics::Shape> new_shape = graphics::MeshCreator::createSurface();
					glm::vec3 center{ m_inputCenter[0], m_inputCenter[1] , m_inputCenter[2] };
					glm::vec3 angle{ 0.0f, 0.0f, 0.0f };
					std::string texture = GUITextureList::s_textures[GUITextureList::s_selectedItem];

					m_meshes[0]->tryReuseShape(new_shape, center, angle, texture);
				}

				if (ImGui::Button("Select Wall")) {
					Ref<graphics::Shape> new_shape = graphics::MeshCreator::createWall();
					glm::vec3 center{ m_inputCenter[0], m_inputCenter[1] , m_inputCenter[2] };
					glm::vec3 angle{ 0.0f, 0.0f, 0.0f };
					std::string texture = GUITextureList::s_textures[GUITextureList::s_selectedItem];

					m_meshes[0]->tryReuseShape(new_shape, center, angle, texture);
				}
			}
			else {
				ImGui::Text("You cannot modify objects!");
			}
		}

		void GUI::Menu_Statistics() {
			if (m_canModifyObjects) {
				std::string drawcalls = "Draw Calls: " + std::to_string(m_statistics->drawCallsCount);
				std::string shapescount = "Shapes Count: " + std::to_string(m_statistics->shapesCount);
				std::string vertices = "Vertices: " + std::to_string(m_statistics->verticesCount);
				std::string indices = "Indices: " + std::to_string(m_statistics->indicesCount);
				std::string triangles = "Triangles: " + std::to_string(m_statistics->trianglesCount);

				ImGui::Text(drawcalls.c_str());
				ImGui::Text(shapescount.c_str());
				ImGui::Text(vertices.c_str());
				ImGui::Text(indices.c_str());
				ImGui::Text(triangles.c_str());
			}
			else {
				ImGui::Text("You cannot modify objects!");
			}

			ImGui::Separator();

			std::string fpsinfo = "FPS: " + std::to_string(ImGui::GetIO().Framerate);
			std::string msframe = "ms/frame: " + std::to_string(1000.0f / ImGui::GetIO().Framerate);

			ImGui::Text(fpsinfo.c_str());
			ImGui::Text(msframe.c_str());
		}

		void GUI::Menu_Info() {
			ImGui::Begin("About");

			std::string aboutEngine = "MAREngine is a AR engine for abstracting 3D models in real world.";
			std::string aboutAuthor = "Mateusz Rzeczyca is C++ / Python programmer and ethusiast of Augmented Reality && Machine Learning.\nHe is a student of Eletronics and Telecommunications at AGH University of Science and Technology";

			ImGui::Text("About Engine");
			ImGui::Text(aboutEngine.c_str());

			ImGui::Text("About Author");
			ImGui::Text(aboutAuthor.c_str());

			if (ImGui::Button("Close")) {
				m_infoWindow = false;
			}

			ImGui::End();
		}

		const glm::mat4 GUI::getTranslationMatrix() const {
			return glm::translate(glm::mat4(1.0f), m_sceneTranslation);
		}

		const glm::mat4 GUI::getRotationMatrix() const {
			return glm::rotate(glm::mat4(1.0f), glm::radians(m_sceneAngle.y), glm::vec3(0.0f, 1.0f, 0.0f))
				 * glm::rotate(glm::mat4(1.0f), glm::radians(m_sceneAngle.z), glm::vec3(0.0f, 0.0f, 1.0f))
				 * glm::rotate(glm::mat4(1.0f), glm::radians(m_sceneAngle.x), glm::vec3(1.0f, 0.0f, 0.0f));
		}


} }