/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "GUI.h"
#include "../Filesystem/EditorFilesystem.h"


namespace mar {
	namespace editor {


		bool GUI::s_dockspaceOpen{ true };
		bool GUI::s_fullscreenPersisant{ true };


		void GUI::initialize(const char* glsl_version) {
			ImGui::CreateContext();
			Setup_Theme();
			ImGui_ImplGlfw_InitForOpenGL(window::Window::getInstance().getWindow(), true);
			ImGui_ImplOpenGL3_Init(glsl_version);

			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
			io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

			EDITOR_INFO("GUI: initialized properly!");
		}

		void GUI::shutdown() {
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

			m_framebuffer.unbind();

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

			Scene_Hierarchy();
			Scene_Entity_Modify();

			Scene_Statistics();
			Display_ViewPort();

			if (m_loadSceneWindow) { Filesystem_LoadScene(); }
			if (m_saveSceneWindow) { Filesystem_SaveScene(); }
			if (m_infoWindow) { Menu_Info(); }
			if (m_instructionWindow) { Menu_Instruction(); }

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

				if (ImGui::BeginMenu("Scene")) {

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

			if (ImGui::IsWindowFocused())
				window::Input::enableInput();
			else
				window::Input::disableInput();

			static graphics::FrameBufferSpecification spec = m_framebuffer.getSpecification();
			static uint32_t id = m_framebuffer.getColorAttach();

			ImVec2 size = ImGui::GetContentRegionAvail();
			spec.width = size.x;
			spec.height = size.y;

			ImGui::Image((void*)id, ImVec2{ spec.width, spec.height }, 
				ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

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

		void GUI::Menu_Info() {
			ImGui::Begin("About");

			const char* aboutEngine = "MAREngine is a AR engine for abstracting 3D models in real world.";
			const char* aboutAuthor = "Mateusz Rzeczyca is C++ / Python programmer and ethusiast of Augmented Reality && Machine Learning.\nHe is a student of Eletronics and Telecommunications at AGH University of Science and Technology";

			ImGui::Text("About Engine");
			ImGui::Text(aboutEngine);

			ImGui::Separator();

			ImGui::Text("About Author");
			ImGui::Text(aboutAuthor);

			if (ImGui::Button("Close")) {
				m_infoWindow = false;
				m_instructionWindow = false;
			}

			ImGui::End();

			EDITOR_TRACE("GUI: menu_info");
		}

		void GUI::Menu_Instruction() {
			ImGui::Begin("Instructions");

			const char* fileManage = "File Management";
			const char* aboutFileManageSave = "Save Method:\nYou have to give the name of file, where you want to write current scene.\nIf file exists, it will be truncated and new scene will be put there.";
			const char* aboutFileManageOpen = "Open Method:\nMethod searches all files in directory resources/mar_files and gets all scenes.\nYou have to click reload button in order to see current state of directory.\nThen you choose one path, which will be loaded into MAREngine.";

			ImGui::Text(fileManage);
			ImGui::Text(aboutFileManageSave);
			ImGui::Text(aboutFileManageOpen);
			ImGui::Separator();

			const char* keyboardSettings = "Keyboard Settings";
			const char* aboutKeySettings = "If Viewport window is selected you can move in scene using WASD keys.\nIf you click Q key, then mouse usage will be enabled! Press it again to disable mouse!";

			ImGui::Text(keyboardSettings);
			ImGui::Text(aboutKeySettings);
			ImGui::Separator();

			const char* mouseSettings = "Mouse Settings";
			const char* aboutMouseSettings = "If you have clicked Q key, you can move around with mouse. Scroll is also available to see things.";

			ImGui::Text(mouseSettings);
			ImGui::Text(aboutMouseSettings);
			ImGui::Separator();

			const char* addShape = "Adding Shapes to Scene";
			const char* aboutAddShape = "In order to add shape to scene, you need to give center of the object. Next point is to select texture.\nIf empty is chosen, shape will be rendered with its default color!";

			ImGui::Text(addShape);
			ImGui::Text(aboutAddShape);
			ImGui::Separator();

			if (ImGui::Button("Close")) {
				m_infoWindow = false;
				m_instructionWindow = false;
			}

			ImGui::End();

			EDITOR_TRACE("GUI: menu_instruction");
		}

		void GUI::submit(ecs::Scene* scene) {
			m_scene = scene;

			EDITOR_INFO("GUI: scene has been submitted!");
		}

		void GUI::Scene_Hierarchy() {
			ImGui::Begin("Scene Hierarchy");

			ImGui::Text(" - ");
			ImGui::SameLine();
			ImGui::Text(m_scene->getName().c_str());

			for (int32_t i = 0; i < (int32_t)m_scene->entities.size(); i++) {
				std::string s = m_scene->entities[i].getComponent<ecs::TagComponent>();
				if (ImGui::MenuItem(s.c_str())) {
					index_entity = i;
				}
			}

			Scene_Hierarchy_PopUp();

			ImGui::End();

			EDITOR_TRACE("GUI: scene_hierarchy");
		}

		void GUI::Scene_Hierarchy_PopUp() {
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
					auto& entity = m_scene->createEntity();
					entity.addComponent<ecs::TransformComponent>(ECS_TRANSFORM);
					index_entity = m_scene->entities.size() - 1;
				}

				ImGui::EndPopup();
			}

			EDITOR_TRACE("GUI: scene_hierarchy_popup");
		}

		void GUI::Scene_Entity_Modify() {
			ImGui::Begin("Entity Modification");

			if (index_entity == -1) {
				ImGui::Text("No entity selected!");
				ImGui::End();
				return;
			}
			
			auto& entity = m_scene->entities[index_entity];

			if (entity.hasComponent<ecs::TagComponent>())
				Scene_Handle_TagComponent();

			if (entity.hasComponent<ecs::TransformComponent>())
				Scene_Handle_TransformComponent();

			if (entity.hasComponent<ecs::RenderableComponent>())
				Scene_Handle_RenderableComponent();
			
			if (entity.hasComponent<ecs::ColorComponent>())
				Scene_Handle_ColorComponent();

			if (entity.hasComponent<ecs::LightComponent>())
				Scene_Handle_LightComponent();

			Scene_Entity_Modify_PopUp();

			ImGui::End();

			EDITOR_TRACE("GUI: scene_entity_modify");
		}

		void GUI::Scene_Entity_Modify_PopUp() {
			static bool b = false;

			// PopMenu should be opened?
			{
				if (ImGui::IsWindowFocused())
					b = window::Input::isMousePressed_NotViewport(MAR_MOUSE_BUTTON_2);
				else
					b = false;

				if (b) {
					ImGui::OpenPopup("SceneEntityModifyPopUp");
					if (window::Input::isMousePressed_NotViewport(MAR_MOUSE_BUTTON_1))
						b = false;
				}
			}
			
			// Actual PopUp menu
			{
				if (ImGui::BeginPopup("SceneEntityModifyPopUp")) {
					auto& entity = m_scene->entities[index_entity];

					if (ImGui::BeginMenu("Add Component")) {
						auto& cmp = entity.getComponent<ecs::Components>();

						for (auto& existing : ecs::AllExistingComponents) {
							
							auto it = std::find(cmp.components.begin(), cmp.components.end(), existing.first);
							if (it == cmp.components.end())
								if (ImGui::MenuItem(existing.second))
									entity.addComponent(existing.first);
							
						}

						ImGui::EndMenu();
					}

					if (ImGui::BeginMenu("Modify Component")) {
						if (entity.hasComponent<ecs::RenderableComponent>()) {
							if (ImGui::MenuItem("RenderableComponent")) {
								modifyRenderable = true;
							}
						}

						ImGui::EndMenu();
					}

					if (ImGui::MenuItem("Delete Entity from Scene")) {
						m_scene->destroyEntity(index_entity);
						index_entity = -1;
					}
				
					ImGui::EndPopup();
				}
			}
			
			EDITOR_TRACE("GUI: scene_entity_modify_popup");
		}

		void GUI::Scene_Handle_TagComponent() {
			ImGui::Text("TagComponent\n");

			static char* input;
			auto& tag = m_scene->entities[index_entity].getComponent<ecs::TagComponent>();

			input = (char*)tag.tag.c_str();

			ImGui::InputText("Tag", input, 25);

			tag.tag = std::string(input);

			EDITOR_TRACE("GUI: SELECTED-ENTITY: handling tag component");
		}

		void GUI::Scene_Handle_TransformComponent() {
			ImGui::Separator();
			ImGui::Text("TransformComponent\n");

			auto& tran = m_scene->entities[index_entity].getComponent<ecs::TransformComponent>();

			// Sliders
			{
				static float last_general;
				last_general = tran.general_scale;

				ImGui::SliderFloat3("Position", maths::vec3::value_ptr_nonconst(tran.center), -15.0f, 15.0f, "%.2f", 1.f);
				ImGui::SliderFloat3("Rotation", maths::vec3::value_ptr_nonconst(tran.angles), -360.f, 360.f, "%.2f", 1.f);
				ImGui::SliderFloat3("Scale", maths::vec3::value_ptr_nonconst(tran.scale), 0.f, 2.0f, "%.2f", 1.f);
				ImGui::SliderFloat("GeneralScale", &tran.general_scale, -2.f, 2.f, "%.2f", 1.f);

				if (last_general != tran.general_scale) 
					tran.scale += tran.general_scale - last_general;

				if (ImGui::Button("Reset to default scale")) {
					tran.general_scale = 1.f;
					tran.scale.x = 1.f;
					tran.scale.y = 1.f;
					tran.scale.z = 1.f;
				}
			}

			ecs::System::handleTransformComponent(tran);

			m_scene->updatedTransforms = true;

			EDITOR_TRACE("GUI: SELECTED-ENTITY: handling transform component");
		}

		void GUI::Scene_Handle_RenderableComponent() {
			ImGui::Separator();
			ImGui::Text("RenderableComponent\n");
			ImGui::SameLine();
			if(!m_scene->entities[index_entity].hasComponent<ecs::ColorComponent>())
				if (ImGui::MenuItem("Remove Renderable")) {
					m_scene->entities[index_entity].removeComponent<ecs::RenderableComponent>(ECS_RENDERABLE);
					m_scene->updatedBuffers = true;
					return;
				}

			auto& renderable = m_scene->entities[index_entity].getComponent<ecs::RenderableComponent>();
			ImGui::Text(renderable.id.c_str());

			if (!m_scene->entities[index_entity].hasComponent<ecs::ColorComponent>()) {
				ImGui::Text("WARNING: Object will not be rendered until you will add ColorComponent!");
			}

			if(renderable.vertices.empty())
				modifyRenderable = true;

			if (modifyRenderable) {
				if (ImGui::Button("Cube")) {
					renderable.id = "Cube";
					renderable.vertices = graphics::MeshCreator::getVertices_Cube();
					renderable.indices = graphics::MeshCreator::getIndices_Cube();
					m_scene->updatedBuffers = true;
					modifyRenderable = false;
				}

				ImGui::SameLine();

				if (ImGui::Button("Pyramid")) {
					renderable.id = "Pyramid";
					renderable.vertices = graphics::MeshCreator::getVertices_Pyramid();
					renderable.indices = graphics::MeshCreator::getIndices_Pyramid();
					m_scene->updatedBuffers = true;
					modifyRenderable = false;
				}

				ImGui::SameLine();

				if (ImGui::Button("Wall")) {
					renderable.id = "Wall";
					renderable.vertices = graphics::MeshCreator::getVertices_Wall();
					renderable.indices = graphics::MeshCreator::getIndices_Wall();
					m_scene->updatedBuffers = true;
					modifyRenderable = false;
				}

				ImGui::SameLine();

				if (ImGui::Button("Surface")) {
					renderable.id = "Surface";
					renderable.vertices = graphics::MeshCreator::getVertices_Surface();
					renderable.indices = graphics::MeshCreator::getIndices_Surface();
					m_scene->updatedBuffers = true;
					modifyRenderable = false;
				}

				ImGui::SameLine();

				if (ImGui::Button("Do not modify")) {
					modifyRenderable = false;
					m_scene->updatedBuffers = false;
				}
			}
			EDITOR_TRACE("GUI: SELECTED-ENTITY: handling renderable component");

		}

		void GUI::Scene_Handle_ColorComponent() {
			ImGui::Separator();
			ImGui::Text("ColorComponent\n");
			ImGui::SameLine();
			if (ImGui::MenuItem("Remove Color")) {
				m_scene->entities[index_entity].removeComponent<ecs::ColorComponent>(ECS_COLOR);
				m_scene->updatedBuffers = true;
				return;
			}

			auto& color = m_scene->entities[index_entity].getComponent<ecs::ColorComponent>();
		
			ImGui::ColorEdit3("- color", maths::vec3::value_ptr_nonconst(color.color));

			m_scene->updatedColors = true;

			EDITOR_TRACE("GUI: SELECTED-ENTITY: handling color component");
		}

		void GUI::Scene_Handle_LightComponent() {
			ImGui::Separator();
			ImGui::Text("LightComponent\n");
			/*
			ImGui::SameLine();
			
			if (ImGui::MenuItem("Remove Light")) {
				m_scene->entities[index_entity].removeComponent<ecs::LightComponent>(ECS_LIGHT);
				return;
			}
			*/

			auto& light = m_scene->entities[index_entity].getComponent<ecs::LightComponent>();

			ImGui::SliderFloat3("Ambient Light", &light.ambient.x, 0.f, 1.f);
			ImGui::SliderFloat3("Diffuse Light", &light.diffuse.x, 0.f, 1.f);
			ImGui::SliderFloat3("Specular Light", &light.specular.x, 0.f, 1.f);

			ImGui::Text("Attenuation");

			ImGui::SliderFloat("Constant", &light.constant, 0.f, 2.f);
			ImGui::SliderFloat("Linear", &light.linear, 0.f, 0.5f);
			ImGui::SliderFloat("Quadratic", &light.quadratic, 0.f, 0.1f);
			ImGui::SliderFloat("Shininess", &light.shininess, 0.f, 256.f);

			m_scene->updatedLight = true;

			EDITOR_TRACE("GUI: SELECTED-ENTITY: handling light component");
		}
		
		void GUI::Filesystem_SaveScene() {
			ImGui::Begin("Save File");

			static char filename[30]{ "empty" };

			ImGui::InputText(".marscene", filename, 30);

			ImGui::Separator();

			static std::string save;
			save = "resources/mar_files/" + std::string(filename) + ".marscene";

			ImGui::Text("Saving to: ");
			ImGui::Text(save.c_str());

			ImGui::Separator();

			if (ImGui::Button("Save to selected name"))
				Filesystem::saveToFile(m_scene, save.c_str());

			ImGui::SameLine();

			if (ImGui::Button("Close"))
				m_saveSceneWindow = false;

			ImGui::End();

			EDITOR_TRACE("GUI: filesystem_savescene");
		}

		void GUI::Filesystem_LoadScene() {
			ImGui::Begin("Open File");

			ImGui::Text("Select file, which you want to be opened:");

			static char input[30];
			ImGui::InputText(".marscene", input, 30);

			ImGui::Separator();

			static std::string will_open;
			will_open = "resources/mar_files/" + std::string(input) + ".marscene";

			ImGui::Text("File, which is going to be opened:");
			ImGui::SameLine();
			ImGui::Text(will_open.c_str());

			ImGui::Text("PLEASE MAKE SURE PATH IS CORRECT!");

			if (ImGui::Button("Open Selected File")) {
				engine::MAREngine::getEngine()->setLoadPath(will_open);
				engine::MAREngine::getEngine()->setRestart();
			}

			ImGui::SameLine();

			if (ImGui::Button("Close"))
				m_loadSceneWindow = false;

			ImGui::End();

			EDITOR_TRACE("GUI: filesystem_loadscene");
		}

		void GUI::Setup_Theme() {
			auto& io = ImGui::GetIO();
			io.Fonts->AddFontFromFileTTF("resources/fonts/Ruda-Bold.ttf", 15.0f);

			ImGuiStyle* style = &ImGui::GetStyle();

			style->WindowPadding = ImVec2(15, 15);
			style->WindowRounding = 5.0f;
			style->FramePadding = ImVec2(5, 5);
			style->FrameRounding = 4.0f;
			style->ItemSpacing = ImVec2(12, 8);
			style->ItemInnerSpacing = ImVec2(8, 6);
			style->IndentSpacing = 25.0f;
			style->ScrollbarSize = 15.0f;
			style->ScrollbarRounding = 9.0f;
			style->GrabMinSize = 5.0f;
			style->GrabRounding = 3.0f;

			style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
			style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
			style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
			style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
			style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
			style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
			style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
			style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
			style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
			style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
			style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
			style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
			style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
			style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
			style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
			style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
			style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
			style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

			EDITOR_INFO("GUI: default theme has been loaded!");
		}


} }