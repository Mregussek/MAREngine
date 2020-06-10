/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "GUI.h"

namespace mar {
	namespace gui {


		GUI::GUI()
			: m_sceneTranslation(glm::vec3(0.0f, 0.0f, 0.0f)),
			m_sceneAngle(glm::vec3(0.0f, 0.0f, 0.0f)),
			m_canModifyObjects(false)
		{}

		void GUI::initialize(window::Window* window, const char* glsl_version, bool can_modify_objects) {
			m_window = window;
			m_canModifyObjects = can_modify_objects;

			ImGui::CreateContext();
			ImGui::StyleColorsDark();
			ImGui_ImplGlfw_InitForOpenGL(m_window->getWindow(), true);
			ImGui_ImplOpenGL3_Init(glsl_version);

			for (auto& c : m_sceneColors) c = 1.0f;
		}

		void GUI::shutdown() {
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		void GUI::prepareNewFrame() {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			m_guiData.translate = getTranslationMatrix();
			m_guiData.rotation = getRotationMatrix();
			m_guiData.colors[0] = m_sceneColors[0];
			m_guiData.colors[1] = m_sceneColors[1];
			m_guiData.colors[2] = m_sceneColors[2];
			m_guiData.colors[3] = m_sceneColors[3];
		}

		void GUI::updateSceneInfo(graphics::Mesh* mesh, const graphics::RendererStatistics* stats) {
			global_mesh = mesh;
			m_statistics = stats;
		}

		void GUI::display() {
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
					if(!m_displayGeneralScene)
						if (ImGui::MenuItem("Open Scene Modification Window")) {
							m_displayGeneralScene = true;
						}

					if (!m_displaySeperatelyShapes)
						if (ImGui::MenuItem("Modify Shapes Separately Window")) {
							m_displaySeperatelyShapes = true;
						}

					if (!m_displayShapePush)
						if (ImGui::MenuItem("Add Shape to Scene Window")) {
							m_displayShapePush = true;
						}

					if (!m_displayStatistics)
						if (ImGui::MenuItem("Scene Statistics Window")) {
							m_displayStatistics = true;
						}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Exit")) {
					glfwSetWindowShouldClose(m_window->getWindow(), true);
					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}

			if (m_displayGeneralScene) { eventOnScene(); }
			if (m_displaySeperatelyShapes) { eventOnEachObjectSeperately(); }
			if (m_displayShapePush) { addNewObjectToScene(); }
			if (m_displayShapePop) { deleteObjectFromScene(); }
			if (m_displayStatistics) { display_StatisticsMenu(); }

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void GUI::eventOnScene() {
			ImGui::Begin("GeneralSceneMenu");

			if (m_canModifyObjects) {
				ImGui::Text("Scene");
				ImGui::SliderFloat3("Translation", &m_sceneTranslation.x, -5.0f, 5.0f);
				ImGui::SliderFloat3("Rotation", &m_sceneAngle.x, -360.0f, 360.0f);
				ImGui::ColorEdit4("color", m_sceneColors);
			}
			else {
				ImGui::Text("You cannot modify objects!");
			}

			ImGui::Separator();

			if (ImGui::Button("Close")) {
				m_displayGeneralScene = false;
			}

			ImGui::End();
		}

		void GUI::eventOnEachObjectSeperately() {
			ImGui::Begin("GeneralShapeSeparatelyMenu");

			if (m_canModifyObjects) {
				ImGui::MenuItem("Objects Menu", "");

				for (unsigned int i = 0; i < global_mesh->getShapesCount(); i++) {
					std::string shapeindex = global_mesh->getName(i) + " " + std::to_string(i);
					std::string shapetrans = "Translate " + global_mesh->getName(i) + " " + std::to_string(i);
					std::string shaperot = "Rotate " + global_mesh->getName(i) + " " + std::to_string(i);

					ImGui::Text(shapeindex.c_str());
					ImGui::SliderFloat3(shapetrans.c_str(), &global_mesh->getCenter(i).x, -5.0f, 5.0f, "%.3f", 0.5f);
					ImGui::SliderFloat3(shaperot.c_str(), &global_mesh->getAngle(i).x, 0.0f, 360.0f, "%.3f", 0.5f);
				}
			}
			else {
				ImGui::Text("You cannot modify objects!");
			}

			ImGui::Separator();

			if (ImGui::Button("Close")) {
				m_displaySeperatelyShapes = false;
			}

			ImGui::End();
		}

		void GUI::addNewObjectToScene() {
			ImGui::Begin("AddShapeToSceneWindow");

			if (m_canModifyObjects) {
				ImGui::Text("Give value for each coordinate, which is in range (-10, 10)");
				ImGui::InputFloat3("Input Center", m_inputCenter);

				if (m_inputCenter[0] > 10.0f || m_inputCenter[0] < -10.0f)
					return;
				else if (m_inputCenter[1] > 10.0f || m_inputCenter[1] < -10.0f)
					return;
				else if (m_inputCenter[2] > 10.0f || m_inputCenter[2] < -10.0f)
					return;

				if (global_mesh->getShapesCount() == graphics::constants::maxObjectsInScene - 1)
					return;

				Ref<graphics::Shape> new_shape;
				bool buttonclicked = false;

				if (ImGui::Button("Select Pyramid")) {
					new_shape = graphics::MeshCreator::createPyramid();
					buttonclicked = true;
				}

				if (ImGui::Button("Select Cube")) {
					new_shape = graphics::MeshCreator::createCube();
					buttonclicked = true;
				}

				if (ImGui::Button("Select Surface")) {
					new_shape = graphics::MeshCreator::createSurface();
					buttonclicked = true;
				}

				if (ImGui::Button("Select Wall")) {
					new_shape = graphics::MeshCreator::createWall();
					buttonclicked = true;
				}

				if (buttonclicked) {
					glm::vec3 center{ m_inputCenter[0], m_inputCenter[1] , m_inputCenter[2] };
					glm::vec3 angle{ 0.0f, 0.0f, 0.0f };
					std::string texture = "empty";

					global_mesh->submitShape(new_shape, center, angle, texture);
				}
			}
			else {
				ImGui::Text("You cannot modify objects!");
			}

			ImGui::Separator();

			if (ImGui::Button("Close")) {
				m_displayShapePush = false;
			}

			ImGui::End();
		}

		void GUI::deleteObjectFromScene() {
			ImGui::Begin("DeleteObjectFromSceneWindow");

			if (m_canModifyObjects) {
				for (unsigned int i = 0; i < global_mesh->getShapesCount(); i++) {
					std::string shapeDel = "Delete " + global_mesh->getName(i) + " " + std::to_string(i);

					if (ImGui::Button(shapeDel.c_str())) {
						//global_mesh->flushShape(i);
						//this->popData(i);
					}
				}
			}
			else {
				ImGui::Text("You cannot modify objects!");
			}

			ImGui::Separator();

			if (ImGui::Button("Close")) {
				m_displayShapePop = false;
			}

			ImGui::End();
		}

		void GUI::display_StatisticsMenu() {
			ImGui::Begin("DisplayStatisticsMenuWindow");

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

			ImGui::Separator();

			if (ImGui::Button("Close")) {
				m_displayStatistics = false;
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