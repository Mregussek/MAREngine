/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "GUI.h"
#include "../../Debug/Log.h"

namespace mar {
	namespace editor {

		std::vector<const char*> GUITextureList::s_textures = { "empty" };
		int GUITextureList::s_selectedItem;

		std::vector<const char*> GUIMarFiles::s_files = { "empty" };
		int GUIMarFiles::s_selectedItem;

		GUIData GUI::s_guiData;
		bool GUI::s_dockspaceOpen{ true };
		bool GUI::s_fullscreenPersisant{ true };


		void GUI::initialize(const char* glsl_version) {
			m_meshIndex = -1;
			m_shapeIndex = -1;

			ImGui::CreateContext();
			Setup_Theme();
			ImGui_ImplGlfw_InitForOpenGL(window::Window::getInstance().getWindow(), true);
			ImGui_ImplOpenGL3_Init(glsl_version);

			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
			io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

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
			ImGui::Begin("MAREngineDockspace", &s_dockspaceOpen, window_flags);
			ImGui::PopStyleVar();

			if (s_fullscreenPersisant)
				ImGui::PopStyleVar(2);

			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
				ImGuiID dockspace_id = ImGui::GetID("MARDockspace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMainMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("Open")) {
						m_fileOpenWindow = true;
					}

					if (ImGui::MenuItem("Save")) {
						m_fileSaveWindow = true;
					}

					if (ImGui::MenuItem("Exit")) {
						window::Window::getInstance().closeWindow();
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Scene")) {
					if (ImGui::BeginMenu("Add Shape to Scene Window")) {
						Menu_PushShapeToScene();
						ImGui::EndMenu();
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Scene Statistics")) {
					Menu_Statistics();
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

			Menu_ModifyScene();
			Menu_SelectShape();
			Menu_ModifyShape();
			Display_ViewPort();

			if (m_fileOpenWindow) { File_Open(); }
			if (m_fileSaveWindow) { File_Save(); }
			if (m_infoWindow) { Menu_Info(); }
			if (m_instructionWindow) { Menu_Instruction(); }

			ImGui::End();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			ImGui::EndFrame();
		}

		void GUI::Setup_Theme() {
			ImGui::GetStyle().FrameRounding = 4.0f;
			ImGui::GetStyle().GrabRounding = 4.0f;

			ImVec4* colors = ImGui::GetStyle().Colors;

			colors[ImGuiCol_Text] =					 ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
			colors[ImGuiCol_TextDisabled] =			 ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
			colors[ImGuiCol_WindowBg] =				 ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
			colors[ImGuiCol_ChildBg] =				 ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
			colors[ImGuiCol_PopupBg] =				 ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
			colors[ImGuiCol_Border] =				 ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
			colors[ImGuiCol_BorderShadow] =			 ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_FrameBg] =				 ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
			colors[ImGuiCol_FrameBgHovered] =		 ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
			colors[ImGuiCol_FrameBgActive] =		 ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
			colors[ImGuiCol_TitleBg] =				 ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
			colors[ImGuiCol_TitleBgActive] =		 ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
			colors[ImGuiCol_TitleBgCollapsed] =		 ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
			colors[ImGuiCol_MenuBarBg] =			 ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
			colors[ImGuiCol_ScrollbarBg] =			 ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
			colors[ImGuiCol_ScrollbarGrab] =		 ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabHovered] =  ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabActive] =	 ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
			colors[ImGuiCol_CheckMark] =			 ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
			colors[ImGuiCol_SliderGrab] =			 ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
			colors[ImGuiCol_SliderGrabActive] =		 ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
			colors[ImGuiCol_Button] =				 ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
			colors[ImGuiCol_ButtonHovered] =		 ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
			colors[ImGuiCol_ButtonActive] =			 ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
			colors[ImGuiCol_Header] =				 ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
			colors[ImGuiCol_HeaderHovered] =		 ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
			colors[ImGuiCol_HeaderActive] =			 ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_Separator] =			 ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
			colors[ImGuiCol_SeparatorHovered] =		 ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
			colors[ImGuiCol_SeparatorActive] =		 ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
			colors[ImGuiCol_ResizeGrip] =			 ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
			colors[ImGuiCol_ResizeGripHovered] =	 ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
			colors[ImGuiCol_ResizeGripActive] =		 ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
			colors[ImGuiCol_Tab] =					 ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
			colors[ImGuiCol_TabHovered] =			 ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
			colors[ImGuiCol_TabActive] =			 ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
			colors[ImGuiCol_TabUnfocused] =			 ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
			colors[ImGuiCol_TabUnfocusedActive] =	 ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
			colors[ImGuiCol_PlotLines] =			 ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
			colors[ImGuiCol_PlotLinesHovered] =		 ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
			colors[ImGuiCol_PlotHistogram] =		 ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered] =  ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			colors[ImGuiCol_TextSelectedBg] =		 ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
			colors[ImGuiCol_DragDropTarget] =		 ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
			colors[ImGuiCol_NavHighlight] =			 ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
			colors[ImGuiCol_NavWindowingDimBg] =	 ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
			colors[ImGuiCol_ModalWindowDimBg] =		 ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		}

		void GUI::File_Open() {
			ImGui::Begin("Open File");

			if (ImGui::Button("Reload files in directory")) {
				filesystem::updateMarFiles("resources/mar_files");

				GUIMarFiles::s_files.clear();
				GUIMarFiles::s_files.push_back("empty");

				for (auto& m : filesystem::getMarFiles()) {
					GUIMarFiles::s_files.push_back(m.c_str());
				}
			}

			ImGui::Separator();

			ImGui::Text("Select file, which you want to be opened:");

			ImGui::Combo("Choose File", &GUIMarFiles::s_selectedItem, GUIMarFiles::s_files.data(), GUIMarFiles::s_files.size());

			ImGui::Separator();

			if (ImGui::Button("Open Selected File")) {
				if (GUIMarFiles::s_selectedItem == 0) {
					MAR_CORE_ERROR("Wrong file selected!");
					return;
				}

				std::string path = "resources/mar_files";
				std::string selected = path + "/" + std::string(GUIMarFiles::s_files[GUIMarFiles::s_selectedItem]);

				engine::MAREngine::getEngine()->setLoadPath(selected);
				engine::MAREngine::getEngine()->setRestart();
			}

			if (ImGui::Button("Close")) 
				m_fileOpenWindow = false;

			ImGui::End();
		}

		void GUI::File_Save() {
			ImGui::Begin("Save File");

			ImGui::InputText(".marscene", m_inputStr, IM_ARRAYSIZE(m_inputStr));

			ImGui::Separator();

			std::string str{ m_inputStr };
			std::string save = "resources/mar_files/" + str + ".marscene";

			ImGui::Text("Saving to: ");
			ImGui::Text(save.c_str());

			ImGui::Separator();

			if (ImGui::Button("Save to selected name")) {
				filesystem::saveSceneToFile(save.c_str(), m_meshes);
			}

			if (ImGui::Button("Close")) {
				m_fileSaveWindow = false;
			}

			ImGui::End();
		}

		void GUI::Display_ViewPort() {
			ImGui::Begin("ViewPort");

			if (ImGui::IsWindowFocused())
				window::Input::enableInput();
			else
				window::Input::disableInput();

			graphics::FrameBufferSpecification spec = m_framebuffer->getSpecification();
			unsigned int id = m_framebuffer->getColorAttach();

			ImVec2 size = ImGui::GetContentRegionAvail();
			spec.width = size.x;
			spec.height = size.y;

			ImGui::Image((void*)id, ImVec2{ spec.width, spec.height }, 
				ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			ImGui::End();
		}

		void GUI::Menu_ModifyScene() {
			ImGui::Begin("Scene Modification Menu");

			if (storage::usegui) {
				ImGui::MenuItem("Scene Menu");

				if (ImGui::TreeNode("Modify Scene")) {
					ImGui::Text("Move Scene");
					ImGui::SliderFloat("TX", &m_sceneTranslation.x, -5.0f, 5.0f, "%.2f", 1.f);
					ImGui::SliderFloat("TY", &m_sceneTranslation.y, -5.0f, 5.0f, "%.2f", 1.f);
					ImGui::SliderFloat("TZ", &m_sceneTranslation.z, -5.0f, 5.0f, "%.2f", 1.f);
					ImGui::Separator();

					ImGui::Text("Rotate Scene");
					ImGui::SliderFloat("RX", &m_sceneAngle.x, -360.f, 360.f, "%.2f", 1.f);
					ImGui::SliderFloat("RY", &m_sceneAngle.y, -360.f, 360.f, "%.2f", 1.f);
					ImGui::SliderFloat("RZ", &m_sceneAngle.z, -360.f, 360.f, "%.2f", 1.f);
					ImGui::Separator();

					ImGui::Text("Color Scene");
					ImGui::ColorEdit4("C", m_sceneColors);

					ImGui::TreePop();
				}
			}
			else {
				ImGui::Text("You cannot modify objects!");
			}

			ImGui::End();
		}

		void GUI::Menu_SelectShape() {
			ImGui::Begin("Select Shape Menu");

			if (storage::usegui) {
				ImGui::MenuItem("Select Shape");

				for (unsigned int index = 0; index < m_meshes.size(); index++) {
					const char* mesh_name = m_meshes[index]->getMeshName();

					if (ImGui::TreeNode(mesh_name)) {
						for (unsigned int i = 0; i < m_meshes[index]->getShapesCount(); i++) {
							const char* shape_name = m_meshes[index]->getShape(i)->getName().c_str();

							if (ImGui::MenuItem(shape_name)) {
								m_meshIndex = index;
								m_shapeIndex = i;
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

			ImGui::End();
		}

		void GUI::Menu_ModifyShape() {
			ImGui::Begin("Modify Shape");

			if (storage::usegui) {

				ImGui::MenuItem("Shapes Menu", "");

				if (m_meshIndex != -1 && m_shapeIndex != -1) {
					auto& center = m_meshes[m_meshIndex]->getShape(m_shapeIndex)->getCenter();
					auto& angle = m_meshes[m_meshIndex]->getShape(m_shapeIndex)->getAngle();
					auto& scale = m_meshes[m_meshIndex]->getShape(m_shapeIndex)->getScaleVec();

					ImGui::Text("Shape");
					ImGui::Separator();

					ImGui::Text("\nTranslate Shape\n");
					ImGui::SliderFloat("X translation", &center.x, -15.0f, 15.0f, "%.2f", 1.f);
					ImGui::SliderFloat("Y translation", &center.y, -15.0f, 15.0f, "%.2f", 1.f);
					ImGui::SliderFloat("Z translation", &center.z, -15.0f, 15.0f, "%.2f", 1.f);
					ImGui::Separator();

					ImGui::Text("\nRotate Shape\n");
					ImGui::SliderFloat("X rotation", &angle.x, -360.f, 360.f, "%.2f", 1.f);
					ImGui::SliderFloat("Y rotation", &angle.y, -360.f, 360.f, "%.2f", 1.f);
					ImGui::SliderFloat("Z rotation", &angle.z, -360.f, 360.f, "%.2f", 1.f);
					ImGui::Separator();

					ImGui::Text("\nScale Shape\n");
					m_generalScale = scale.x;
					ImGui::SliderFloat("General Scale\n", &m_generalScale, 0.f, 2.f, "%.2f", 1.f);
					scale.x = m_generalScale;
					scale.y = m_generalScale;
					scale.z = m_generalScale;

					ImGui::Separator();

					if (m_meshes[m_meshIndex]->getMeshType() != graphics::MeshType::CUBEMAPS) {
						if (m_meshes[m_meshIndex]->getTexture()->getPath(m_shapeIndex) != "empty") {
							ImGui::Image((void*)m_meshes[m_meshIndex]->getTexture()->getID(m_shapeIndex),
								ImVec2(100, 100), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
						}
						else {
							ImGui::ColorPicker3("Color shape", (float*)&m_meshes[m_meshIndex]->getColors()[m_shapeIndex]);
						}
					}
						
					ImGui::Separator();
					if (ImGui::Button("Delete Shape")) {
						m_meshes[m_meshIndex]->flushShape(m_shapeIndex);

						m_meshIndex = -1;
						m_shapeIndex = -1;
					}
				}
				else { ImGui::Text("No shape selected!"); }
			}
			else { ImGui::Text("You cannot modify objects!"); }

			ImGui::End();
		}

		void GUI::Menu_PushShapeToScene() {
			if (storage::usegui) {
				ImGui::Text("Give value for each coordinate, which is in range (-10, 10)");
				ImGui::InputFloat3("Input Center", m_inputCenter);

				for (unsigned int i = 0; i < 3; i++)
					if (m_inputCenter[i] > 10.f || m_inputCenter[i] < -10.f)
						return;

				ImGui::Combo("Choose Mesh", &m_pushMeshIndex, m_meshesNames.data(), m_meshesNames.size());

				ImGui::Separator();

				if (ImGui::Button("Reload textures in directory")) {
					filesystem::updateMarTextures("resources/textures");

					GUITextureList::s_textures.clear();
					GUITextureList::s_textures.push_back("empty");

					for (auto& m : filesystem::getMarTextures()) {
						GUITextureList::s_textures.push_back(m.c_str());
					}
				}

				ImGui::Combo("Choose Texture", &GUITextureList::s_selectedItem, 
					GUITextureList::s_textures.data(), GUITextureList::s_textures.size());

				ImGui::Separator();

				if (checkCharsStart("Objects", m_meshesNames[m_pushMeshIndex])) {
					ImGui::Text("MAREngine do not support push to objects!");
					return;
				}
					
				if (ImGui::Button("Select Pyramid")) {
					Ref<graphics::Shape> new_shape = graphics::MeshCreator::createPyramid();
					maths::vec3 center{ m_inputCenter[0], m_inputCenter[1] , m_inputCenter[2] };
					maths::vec3 angle{ 0.0f, 0.0f, 0.0f };
					maths::vec3 scale{ 1.f, 1.f, 1.f };

					std::string path = "resources/textures";
					std::string selected = path + "/" + std::string(GUITextureList::s_textures[GUITextureList::s_selectedItem]);

					m_meshes[m_pushMeshIndex]->tryReuseShape(new_shape, center, angle, scale, selected.c_str());
				}

				if (ImGui::Button("Select Cube")) {
					Ref<graphics::Shape> new_shape = graphics::MeshCreator::createCube();
					maths::vec3 center{ m_inputCenter[0], m_inputCenter[1] , m_inputCenter[2] };
					maths::vec3 angle{ 0.0f, 0.0f, 0.0f };
					maths::vec3 scale{ 1.f, 1.f, 1.f };

					std::string path = "resources/textures";
					std::string selected = path + "/" + std::string(GUITextureList::s_textures[GUITextureList::s_selectedItem]);

					m_meshes[m_pushMeshIndex]->tryReuseShape(new_shape, center, angle, scale, selected.c_str());
				}

				if (ImGui::Button("Select Surface")) {
					Ref<graphics::Shape> new_shape = graphics::MeshCreator::createSurface();
					maths::vec3 center{ m_inputCenter[0], m_inputCenter[1] , m_inputCenter[2] };
					maths::vec3 angle{ 0.0f, 0.0f, 0.0f };
					maths::vec3 scale{ 1.f, 1.f, 1.f };

					std::string path = "resources/textures";
					std::string selected = path + "/" + std::string(GUITextureList::s_textures[GUITextureList::s_selectedItem]);

					m_meshes[m_pushMeshIndex]->tryReuseShape(new_shape, center, angle, scale, selected.c_str());
				}

				if (ImGui::Button("Select Wall")) {
					Ref<graphics::Shape> new_shape = graphics::MeshCreator::createWall();
					maths::vec3 center{ m_inputCenter[0], m_inputCenter[1] , m_inputCenter[2] };
					maths::vec3 angle{ 0.0f, 0.0f, 0.0f };
					maths::vec3 scale{ 1.f, 1.f, 1.f };

					std::string path = "resources/textures";
					std::string selected = path + "/" + std::string(GUITextureList::s_textures[GUITextureList::s_selectedItem]);

					m_meshes[m_pushMeshIndex]->tryReuseShape(new_shape, center, angle, scale, selected.c_str());
				}
			}
			else {
				ImGui::Text("You cannot modify objects!");
			}
		}

		void GUI::Menu_Statistics() {
			if (storage::usegui) {
				using stats = graphics::Renderer;

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

			const char* aboutEngine = "MAREngine is a AR engine for abstracting 3D models in real world.";
			const char* aboutAuthor = "Mateusz Rzeczyca is C++ / Python programmer and ethusiast of Augmented Reality && Machine Learning.\nHe is a student of Eletronics and Telecommunications at AGH University of Science and Technology";

			ImGui::Text("About Engine");
			ImGui::Text(aboutEngine);

			ImGui::Text("About Author");
			ImGui::Text(aboutAuthor);

			if (ImGui::Button("Close")) {
				m_infoWindow = false;
				m_instructionWindow = false;
			}

			ImGui::End();
		}

		void GUI::Menu_Instruction() {
			ImGui::Begin("Instructions");

			const char* fileManage = "File Management";
			const char* aboutFileManageSave = "\nSave Method:\nYou have to give the name of file, where you want to write current scene.\nIf file exists, it will be truncated and new scene will be put there.";
			const char* aboutFileManageOpen = "\nOpen Method:\nMethod searches all files in directory resources/mar_files and gets all scenes.\nYou have to click reload button in order to see current state of directory.\nThen you choose one path, which will be loaded into MAREngine.";

			ImGui::Text(fileManage);
			ImGui::Text(aboutFileManageSave);
			ImGui::Text(aboutFileManageOpen);
			ImGui::Separator();

			const char* keyboardSettings = "\nKeyboard Settings";
			const char* aboutKeySettings = "\nIf Viewport window is selected you can move in scene using WASD keys.\nIf you click Q key, then mouse usage will be enabled! Press it again to disable mouse!";

			ImGui::Text(keyboardSettings);
			ImGui::Text(aboutKeySettings);
			ImGui::Separator();

			const char* mouseSettings = "\nMouse Settings";
			const char* aboutMouseSettings = "\nIf you have clicked Q key, you can move around with mouse. Scroll is also available to see things.";

			ImGui::Text(mouseSettings);
			ImGui::Text(aboutMouseSettings);
			ImGui::Separator();

			const char* addShape = "\nAdding Shapes to Scene";
			const char* aboutAddShape = "\nIn order to add shape to scene, you need to give center of the object. Next point is to select texture.\nIf empty is chosen, shape will be rendered with its default color!";

			ImGui::Text(addShape);
			ImGui::Text(aboutAddShape);
			ImGui::Separator();

			if (ImGui::Button("Close")) {
				m_infoWindow = false;
				m_instructionWindow = false;
			}

			ImGui::End();
		}

		const maths::mat4 GUI::getTranslationMatrix() const {
			return maths::mat4::translation(m_sceneTranslation);
		}

		const maths::mat4 GUI::getRotationMatrix() const {
			return maths::mat4::rotation(maths::Trig::toRadians(m_sceneAngle.y), maths::vec3(0.0f, 1.0f, 0.0f))
				 * maths::mat4::rotation(maths::Trig::toRadians(m_sceneAngle.z), maths::vec3(0.0f, 0.0f, 1.0f))
				 * maths::mat4::rotation(maths::Trig::toRadians(m_sceneAngle.x), maths::vec3(1.0f, 0.0f, 0.0f));
		}


} }