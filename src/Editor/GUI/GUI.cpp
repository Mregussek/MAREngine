/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "GUI.h"
#include "../../Debug/Log.h"
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

			MAR_CORE_INFO("GUI has initialized properly!");
		}

		void GUI::shutdown() {
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();

			MAR_CORE_INFO("GUI is closed!");
		}

		void GUI::display() {
			prepareNewFrame();
			updateFrame();
			endFrame();
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
		}

		void GUI::updateFrame() {
			Menu_MainMenuBar();

			Scene_Hierarchy();
			Scene_Entity_Modify();

			Scene_Statistics();
			Display_ViewPort();

			if (m_saveSceneWindow) { Filesystem_SaveScene(); }
			if (m_infoWindow) { Menu_Info(); }
			if (m_instructionWindow) { Menu_Instruction(); }
		}

		void GUI::endFrame() {
			ImGui::End();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			ImGui::EndFrame();
		}

		void GUI::Menu_MainMenuBar() {
			if (ImGui::BeginMainMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("Open")) {

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

		void GUI::submit(ecs::Scene* scene) {
			if (m_scenes.size() == 1) {
				MAR_CORE_ERROR("Cannot push more scenes! You must pop the one existing!");
				return;
			}

			m_scenes.push_back(scene);
		}

		void GUI::Scene_Hierarchy() {
			ImGui::Begin("Scene Hierarchy");

			for (int32_t j = 0; j < (int32_t)m_scenes.size(); j++) {
				
				ImGui::Text(" - ");
				ImGui::SameLine();
				ImGui::Text(m_scenes[j]->getName());

				for (int32_t i = 0; i < (int32_t)m_scenes[j]->entities.size(); i++) {
					std::string s = m_scenes[j]->entities[i].getComponent<ecs::TagComponent>();
					if (ImGui::MenuItem(s.c_str())) {
						index_scene = j;
						index_entity = i;
					}
				}
				
			}

			ImGui::End();
		}

		void GUI::Scene_Entity_Modify() {
			ImGui::Begin("Entity Modification");

			if (index_scene == -1 || index_entity == -1) {
				ImGui::Text("No entity selected!");
				ImGui::End();
				return;
			}
			
			auto& entity = m_scenes[index_scene]->entities[index_entity];

			if (entity.hasComponent<ecs::TagComponent>())
				Scene_Handle_TagComponent();

			if (entity.hasComponent<ecs::RenderableComponent>()) {
				ImGui::Separator();
				ImGui::Text("\nRenderableComponent\n");
				auto& renderable = entity.getComponent<ecs::RenderableComponent>();
				ImGui::Text(renderable.id.c_str());
				ImGui::Text("\n");
			}

			if (entity.hasComponent<ecs::TransformComponent>())
				Scene_Handle_TransformComponent();
			
			if (entity.hasComponent<ecs::ColorComponent>())
				Scene_Handle_ColorComponent();

			if (entity.hasComponent<ecs::LightComponent>())
				Scene_Handle_LightComponent();

			ImGui::End();
		}

		void GUI::Scene_Handle_TagComponent() {
			ImGui::Separator();
			ImGui::Text("\nTagComponent\n");

			auto& tag = m_scenes[index_scene]->entities[index_entity].getComponent<ecs::TagComponent>();

			std::string s = tag.tag;

			ImGui::InputText("- Tag", (char*)s.c_str(), 30);

			tag.tag = s;

			ImGui::Text("\n");
		}

		void GUI::Scene_Handle_TransformComponent() {
			ImGui::Separator();
			ImGui::Text("\nTransformComponent\n");

			auto& tran = m_scenes[index_scene]->entities[index_entity].getComponent<ecs::TransformComponent>();

			// Sliders
			{
				ImGui::Text("Position\n");
				ImGui::SliderFloat3("pos", maths::vec3::value_ptr_nonconst(tran.center), -15.0f, 15.0f, "%.2f", 1.f);
				
				ImGui::Text("Rotation\n");
				ImGui::SliderFloat3("rot", maths::vec3::value_ptr_nonconst(tran.angles), -360.f, 360.f, "%.2f", 1.f);
				
				ImGui::Text("Scale");

				static float last_general;
				last_general = tran.general_scale;

				ImGui::SliderFloat("General\n", &tran.general_scale, -2.f, 2.f, "%.2f", 1.f);
				if (last_general != tran.general_scale) {
					tran.scale += tran.general_scale - last_general;
				}

				ImGui::SliderFloat3("scale", maths::vec3::value_ptr_nonconst(tran.scale), 0.f, 2.0f, "%.2f", 1.f);

				if (ImGui::Button("Reset to default scale")) {
					tran.general_scale = 1.f;
					tran.scale.x = 1.f;
					tran.scale.y = 1.f;
					tran.scale.z = 1.f;
				}
			}
			
			ecs::System::handleTransformComponent(tran);

			m_scenes[index_scene]->updatedTransforms = true;

			ImGui::Text("\n");
		}

		void GUI::Scene_Handle_ColorComponent() {
			ImGui::Separator();
			ImGui::Text("\nColorComponent\n");

			auto& color = m_scenes[index_scene]->entities[index_entity].getComponent<ecs::ColorComponent>();
		
			ImGui::ColorEdit3("- color", maths::vec3::value_ptr_nonconst(color.color));

			m_scenes[index_scene]->updatedColors = true;

			ImGui::Text("\n");
		}

		void GUI::Scene_Handle_LightComponent() {
			ImGui::Separator();
			ImGui::Text("\nLightComponent\n");

			auto& light = m_scenes[index_scene]->entities[index_entity].getComponent<ecs::LightComponent>();

			ImGui::Text("Ambient Light");

			ImGui::SliderFloat3("A", &light.ambient.x, 0.f, 1.f);

			ImGui::Text("Ambient Strength");

			ImGui::SliderFloat3("SA", &light.ambientStrength.x, 0.f, 1.f);

			ImGui::Text("Diffuse Light");

			ImGui::SliderFloat3("D", &light.diffuse.x, 0.f, 1.f);

			ImGui::Text("Diffuse Strength");

			ImGui::SliderFloat3("SD", &light.diffuseStrength.x, 0.f, 1.f);

			ImGui::Text("Specular Light");

			ImGui::SliderFloat3("S", &light.specular.x, 0.f, 1.f);

			ImGui::Text("Specular Strength");

			ImGui::SliderFloat3("SS", &light.specularStrength.x, 0.f, 1.f);

			ImGui::Text("Attenuation");

			ImGui::InputFloat("Constant", &light.constant);
			ImGui::InputFloat("Linear", &light.linear);
			ImGui::InputFloat("Quadratic", &light.quadratic);
			ImGui::InputFloat("Shininess", &light.shininess);

			m_scenes[index_scene]->updatedLight = true;

			ImGui::Text("\n");
		}
		
		void GUI::Filesystem_SaveScene() {
			ImGui::Begin("Save File");

			static char filename[30]{ "empty" };

			ImGui::InputText(".marscene", filename, 30);

			ImGui::Separator();

			static std::string save;
			save = "resources/mar_files/" + std::string(filename) + ".marscene";

			ImGui::Text("\nSaving to: ");
			ImGui::Text(save.c_str());

			ImGui::Separator();

			if (ImGui::Button("Save to selected name"))
				Filesystem::saveToFile(m_scenes[0], save.c_str());

			if (ImGui::Button("Close"))
				m_saveSceneWindow = false;

			ImGui::End();
		}

		void GUI::Setup_Theme() {
			ImGui::GetStyle().FrameRounding = 4.0f;
			ImGui::GetStyle().GrabRounding = 4.0f;

			ImVec4* colors = ImGui::GetStyle().Colors;

			colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
			colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
			colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
			colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
			colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
			colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
			colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
			colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
			colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
			colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
			colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
			colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
			colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
			colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
			colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
			colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
			colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
			colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
			colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
			colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
			colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
			colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
			colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
			colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
			colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
			colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
			colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
			colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
			colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
			colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
			colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
			colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
			colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		}

} }