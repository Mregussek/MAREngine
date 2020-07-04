/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "GUI.h"
#include "../Debug/Log.h"

namespace mar {
	namespace gui {

		std::vector<const char*> GUITextureList::s_textures = {
			"empty",
			TexturePaths::mrTex,
			TexturePaths::redTex,
			TexturePaths::blueTex,
			TexturePaths::blackTex,
			TexturePaths::yellowTex,
			TexturePaths::wallTex,
			TexturePaths::grassTex 
		};
		int GUITextureList::s_selectedItem;

		std::vector<const char*> GUIMarFiles::s_files = {
			"empty"
		};
		int GUIMarFiles::s_selectedItem;

		GUIData GUI::s_guiData;
		bool GUI::s_dockspaceOpen{ true };
		bool GUI::s_fullscreenPersisant{ true };


		void GUI::initialize(window::Window* window, const char* glsl_version, bool can_modify_objects) {
			m_window = window;
			m_canModifyObjects = can_modify_objects;

			m_meshIndex = -1;
			m_shapeIndex = -1;

			ImGui::CreateContext();
			ImGui::StyleColorsDark();
			ImGui_ImplGlfw_InitForOpenGL(m_window->getWindow(), true);
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
					if (ImGui::MenuItem("Open")) {
						m_fileOpenWindow = true;
					}

					if (ImGui::MenuItem("Save")) {
						m_fileSaveWindow = true;
					}

					if (ImGui::MenuItem("Exit")) {
						m_window->closeWindow();
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

				ImGui::EndMainMenuBar();
			}

			Menu_ModifyScene();
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

		void GUI::File_Open() {
			ImGui::Begin("Open File");

			if (ImGui::Button("Reload files in directory")) {
				filesystem::fnc::updateMarFiles("resources/mar_files");

				GUIMarFiles::s_files.clear();
				GUIMarFiles::s_files.push_back("empty");

				for (auto& m : filesystem::fnc::getMarFiles()) {
					GUIMarFiles::s_files.push_back(m.c_str());
				}
			}

			ImGui::Separator();

			ImGui::Text("Select file, which you want to be opened:");

			ImGui::ListBox("Choose File", &GUIMarFiles::s_selectedItem, GUIMarFiles::s_files.data(), GUIMarFiles::s_files.size());

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
				filesystem::fnc::saveSceneToFile(save.c_str(), m_meshes);
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

			if (m_canModifyObjects) {
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

				ImGui::MenuItem("Select Shape");

				for (unsigned int index = 0; index < m_meshes.size(); index++) {
					std::string mesh;

					switch (m_meshes[index]->getMeshType()) {
					case graphics::MeshType::NORMAL: mesh = "Default Mesh " + std::to_string(index);
						break;
					case graphics::MeshType::CUBEMAPS: mesh = "Cubemap Mesh " + std::to_string(index);
						break;
					case graphics::MeshType::OBJECTS: mesh = "Object Mesh " + std::to_string(index);
						break;
					}

					if (ImGui::TreeNode(mesh.c_str())) {
						for (unsigned int i = 0; i < m_meshes[index]->getShapesCount(); i++) {
							std::string name = m_meshes[index]->getShape(i)->getName();
							std::string shapeindex = mesh + " " + name + " " +std::to_string(i);

							if (ImGui::MenuItem(shapeindex.c_str())) {
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

			if (m_canModifyObjects) {

				ImGui::MenuItem("Shapes Menu", "");

				if (m_meshIndex != -1 && m_shapeIndex != -1) {
					const char* shape = "Shape";
					const char* shape_trans = "\nTranslate Shape\n";
					const char* shape_rot = "\nRotate Shape\n";
					const char* shape_scale = "\nScale Shape\n";
					const char* general_scale = "General Scale\n";
					const char* delete_shape = "Delete Shape";
					const char* x_trans = "X translation";
					const char* y_trans = "Y translation";
					const char* z_trans = "Z translation";
					const char* x_rot = "X rotation";
					const char* y_rot = "Y rotation";
					const char* z_rot = "Z rotation";
					const char* color_shape = "Color shape";

					glm::vec3& center = m_meshes[m_meshIndex]->getShape(m_shapeIndex)->getCenter();
					glm::vec3& angle = m_meshes[m_meshIndex]->getShape(m_shapeIndex)->getAngle();
					glm::vec3& scale = m_meshes[m_meshIndex]->getShape(m_shapeIndex)->getScale();

					ImGui::Text(shape);
					ImGui::Separator();

					ImGui::Text(shape_trans);
					ImGui::SliderFloat(x_trans, &center.x, -15.0f, 15.0f, "%.2f", 1.f);
					ImGui::SliderFloat(y_trans, &center.y, -15.0f, 15.0f, "%.2f", 1.f);
					ImGui::SliderFloat(z_trans, &center.z, -15.0f, 15.0f, "%.2f", 1.f);
					ImGui::Separator();

					ImGui::Text(shape_rot);
					ImGui::SliderFloat(x_rot, &angle.x, -360.f, 360.f, "%.2f", 1.f);
					ImGui::SliderFloat(y_rot, &angle.y, -360.f, 360.f, "%.2f", 1.f);
					ImGui::SliderFloat(z_rot, &angle.z, -360.f, 360.f, "%.2f", 1.f);
					ImGui::Separator();

					ImGui::Text(shape_scale);
					m_generalScale = scale.x;
					ImGui::SliderFloat(general_scale, &m_generalScale, 0.f, 2.f, "%.2f", 1.f);
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
							ImGui::ColorPicker3(color_shape, (float*)&m_meshes[m_meshIndex]->getColors()[m_shapeIndex]);
						}
					}
						

					ImGui::Separator();
					if (ImGui::Button(delete_shape)) {
						m_meshes[m_meshIndex]->flushShape(m_shapeIndex);

						m_meshIndex = -1;
						m_shapeIndex = -1;
					}
				}
				else {
					ImGui::Text("No shape selected!");
				}
			}
			else {
				ImGui::Text("You cannot modify objects!");
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

				if (m_meshes[0]->getShapesCount() == constants::maxObjectsInScene - 1)
					return;

				ImGui::Combo("Choose Texture", &GUITextureList::s_selectedItem, 
					GUITextureList::s_textures.data(), GUITextureList::s_textures.size());

				ImGui::Separator();

				if (ImGui::Button("Select Pyramid")) {
					Ref<graphics::Shape> new_shape = graphics::MeshCreator::createPyramid();
					glm::vec3 center{ m_inputCenter[0], m_inputCenter[1] , m_inputCenter[2] };
					glm::vec3 angle{ 0.0f, 0.0f, 0.0f };
					glm::vec3 scale{ 1.f, 1.f, 1.f };

					m_meshes[0]->tryReuseShape(new_shape, center, angle, scale, GUITextureList::s_textures[GUITextureList::s_selectedItem]);
				}

				if (ImGui::Button("Select Cube")) {
					Ref<graphics::Shape> new_shape = graphics::MeshCreator::createCube();
					glm::vec3 center{ m_inputCenter[0], m_inputCenter[1] , m_inputCenter[2] };
					glm::vec3 angle{ 0.0f, 0.0f, 0.0f };
					glm::vec3 scale{ 1.f, 1.f, 1.f };

					m_meshes[0]->tryReuseShape(new_shape, center, angle, scale, GUITextureList::s_textures[GUITextureList::s_selectedItem]);
				}

				if (ImGui::Button("Select Surface")) {
					Ref<graphics::Shape> new_shape = graphics::MeshCreator::createSurface();
					glm::vec3 center{ m_inputCenter[0], m_inputCenter[1] , m_inputCenter[2] };
					glm::vec3 angle{ 0.0f, 0.0f, 0.0f };
					glm::vec3 scale{ 1.f, 1.f, 1.f };

					m_meshes[0]->tryReuseShape(new_shape, center, angle, scale, GUITextureList::s_textures[GUITextureList::s_selectedItem]);
				}

				if (ImGui::Button("Select Wall")) {
					Ref<graphics::Shape> new_shape = graphics::MeshCreator::createWall();
					glm::vec3 center{ m_inputCenter[0], m_inputCenter[1] , m_inputCenter[2] };
					glm::vec3 angle{ 0.0f, 0.0f, 0.0f };
					glm::vec3 scale{ 1.f, 1.f, 1.f };

					m_meshes[0]->tryReuseShape(new_shape, center, angle, scale, GUITextureList::s_textures[GUITextureList::s_selectedItem]);
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

		const glm::mat4 GUI::getTranslationMatrix() const {
			return glm::translate(glm::mat4(1.0f), m_sceneTranslation);
		}

		const glm::mat4 GUI::getRotationMatrix() const {
			return glm::rotate(glm::mat4(1.0f), glm::radians(m_sceneAngle.y), glm::vec3(0.0f, 1.0f, 0.0f))
				 * glm::rotate(glm::mat4(1.0f), glm::radians(m_sceneAngle.z), glm::vec3(0.0f, 0.0f, 1.0f))
				 * glm::rotate(glm::mat4(1.0f), glm::radians(m_sceneAngle.x), glm::vec3(1.0f, 0.0f, 0.0f));
		}


} }