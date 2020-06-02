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

		void GUI::loadSceneParameters(graphics::Scene* scene) {
			for (unsigned int i = 0; i < scene->getShapesNumber(); i++) {
				push(scene->getCenter(i), scene->getAngle(i));
			}
		}

		void GUI::push(const glm::vec3& new_center, const glm::vec3& new_angle) {
			m_guiData.centers.push_back(new_center);
			m_guiData.angles.push_back(new_angle);
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
				if (ImGui::BeginMenu("General")) {
					display_GeneralMenu();
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Manage Objects")) {
					display_ManageObjectsMenu();
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Statistics")) {
					display_StatisticsMenu();
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Exit")) {
					glfwSetWindowShouldClose(m_window->getWindow(), true);
					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void GUI::display_GeneralMenu() {
			if (m_canModifyObjects) {
				eventOnScene();

				ImGui::Separator();

				eventOnEachObjectSeperately();
			}
			else {
				ImGui::Text("You cannot modify objects!");
			}

			ImGui::Separator();

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			ImGui::Separator();

			if (ImGui::Button("Exit"))
				glfwSetWindowShouldClose(m_window->getWindow(), true);
		}

		void GUI::eventOnScene() {
			ImGui::Text("Scene");
			ImGui::SliderFloat3("Translation", &m_sceneTranslation.x, -5.0f, 5.0f);
			ImGui::SliderFloat3("Rotation", &m_sceneAngle.x, -360.0f, 360.0f);
			ImGui::ColorEdit4("color", m_sceneColors);
		}

		void GUI::eventOnEachObjectSeperately() {
			ImGui::MenuItem("Objects Menu", "");

			for (unsigned int i = 0; i < m_guiData.centers.size(); i++) {
				// Set current variables to sliders
				m_shapePos[0] = m_guiData.centers[i].x;
				m_shapePos[1] = m_guiData.centers[i].y;
				m_shapePos[2] = m_guiData.centers[i].z;
				m_shapeAngle[0] = m_guiData.angles[i].x;
				m_shapeAngle[1] = m_guiData.angles[i].y;
				m_shapeAngle[2] = m_guiData.angles[i].z;

				// Prepare index on GUI
				char int2char[5];
				sprintf_s(int2char, " %d", i);

				char shapeIndex[25] = "";
				strcat_s(shapeIndex, global_mesh->getName(i).c_str());

				strcat_s(shapeIndex, int2char);
				ImGui::Text(shapeIndex);

				// Change center by GUI
				char DragID[15] = "ObjPos ";
				strcat_s(DragID, int2char);
				ImGui::SliderFloat3(DragID, m_shapePos, -5.0f, 5.0f);

				// Change angle by GUI
				char RotID[15] = "ObjRot ";
				strcat_s(RotID, int2char);
				ImGui::SliderFloat3(RotID, m_shapeAngle, 0.0f, 360.0f);

				// Set new variables to object
				m_guiData.centers[i].x = m_shapePos[0];
				m_guiData.centers[i].y = m_shapePos[1];
				m_guiData.centers[i].z = m_shapePos[2];
				m_guiData.angles[i].x = m_shapeAngle[0];
				m_guiData.angles[i].y = m_shapeAngle[1];
				m_guiData.angles[i].z = m_shapeAngle[2];
			}
		}

		void GUI::display_ManageObjectsMenu() {
			if (m_canModifyObjects) {
				addNewObjectToScene();

				ImGui::Separator();

				deleteObjectFromScene();

			}
			else {
				ImGui::Text("You cannot modify objects!");
			}
		}

		void GUI::addNewObjectToScene() {
			ImGui::Text("Add Object");
			ImGui::Text("Give value for each coordinate, which is in range (-10, 10)");
			ImGui::InputFloat3("Input Center", m_inputCenter);

			if (m_inputCenter[0] > 10.0f || m_inputCenter[0] < -10.0f)
				return;
			if (m_inputCenter[1] > 10.0f || m_inputCenter[1] < -10.0f)
				return;
			if (m_inputCenter[2] > 10.0f || m_inputCenter[2] < -10.0f)
				return;

			if (ImGui::Button("Select Pyramid")) {
				if (m_guiData.centers.size() == graphics::constants::maxObjectsInScene)
					return;

				auto new_shape = graphics::MeshCreator::createPyramid();
				glm::vec3 center{ m_inputCenter[0], m_inputCenter[1] , m_inputCenter[2] };
				glm::vec3 angle{ 0.0f, 0.0f, 0.0f };
				std::string texture = "empty";

				global_mesh->submitShape(new_shape, center, angle, texture);
				this->push(center, angle);
			}

			if (ImGui::Button("Select Cube")) {
				if (m_guiData.centers.size() == graphics::constants::maxObjectsInScene)
					return;

				auto new_shape = graphics::MeshCreator::createCube();
				glm::vec3 center{ m_inputCenter[0], m_inputCenter[1] , m_inputCenter[2] };
				glm::vec3 angle{ 0.0f, 0.0f, 0.0f };
				std::string texture = "empty";

				global_mesh->submitShape(new_shape, center, angle, texture);
				this->push(center, angle);
			}

			if (ImGui::Button("Select Surface")) {
				if (m_guiData.centers.size() == graphics::constants::maxObjectsInScene)
					return;

				auto new_shape = graphics::MeshCreator::createSurface();
				glm::vec3 center{ m_inputCenter[0], m_inputCenter[1] , m_inputCenter[2] };
				glm::vec3 angle{ 0.0f, 0.0f, 0.0f };
				std::string texture = "empty";

				global_mesh->submitShape(new_shape, center, angle, texture);
				this->push(center, angle);
			}

			if (ImGui::Button("Select Wall")) {
				if (m_guiData.centers.size() == graphics::constants::maxObjectsInScene)
					return;

				auto new_shape = graphics::MeshCreator::createWall();
				glm::vec3 center{ m_inputCenter[0], m_inputCenter[1] , m_inputCenter[2] };
				glm::vec3 angle{ 0.0f, 0.0f, 0.0f };
				std::string texture = "empty";

				global_mesh->submitShape(new_shape, center, angle, texture);
				this->push(center, angle);
			}
		}

		void GUI::deleteObjectFromScene() {
			ImGui::MenuItem("Delete Object", "");

			for (unsigned int i = 0; i < m_guiData.centers.size(); i++) {
				char int2char[5];
				sprintf_s(int2char, " %d ", i);
				char shapeIndex[30] = " Delete ";

				strcat_s(shapeIndex, global_mesh->getName(i).c_str());
				strcat_s(shapeIndex, int2char);

				if (ImGui::Button(shapeIndex)) {
					global_mesh->flushShape(i);
					m_guiData.centers.erase(m_guiData.centers.begin() + i);
					m_guiData.angles.erase(m_guiData.angles.begin() + i);
				}
			}
		}

		void GUI::display_StatisticsMenu() {
			if (m_canModifyObjects) {
				char int2char[7];
				sprintf_s(int2char, "%d ", m_statistics->_countOfDrawCalls);
				char drawCalls[28] = "Draw Calls: ";
				strcat_s(drawCalls, int2char);
				ImGui::Text(drawCalls);

				char int2char1[7];
				sprintf_s(int2char1, "%d ", m_statistics->_countOfShapes);
				char drawCalls1[28] = "Shapes Count: ";
				strcat_s(drawCalls1, int2char1);
				ImGui::Text(drawCalls1);

				char int2char2[7];
				sprintf_s(int2char2, "%d ", m_statistics->_countOfVertices);
				char drawCalls2[28] = "Vertices: ";
				strcat_s(drawCalls2, int2char2);
				ImGui::Text(drawCalls2);

				char int2char3[7];
				sprintf_s(int2char3, "%d ", m_statistics->_countOfIndices);
				char drawCalls3[28] = "Indices: ";
				strcat_s(drawCalls3, int2char3);
				ImGui::Text(drawCalls3);

				char int2char4[7];
				sprintf_s(int2char4, "%d ", m_statistics->_countOfTriangles);
				char drawCalls4[28] = "Triangles: ";
				strcat_s(drawCalls4, int2char4);
				ImGui::Text(drawCalls4);
			}
			else {
				ImGui::Text("You cannot modify objects!");
			}
		}

		const glm::mat4 GUI::getTranslationMatrix() const {
			return glm::translate(glm::mat4(1.0f), m_sceneTranslation);
		}

		const glm::mat4 GUI::getRotationMatrix() const {
			glm::mat4 rotation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			return glm::rotate(rotation, glm::radians(m_sceneAngle.x), glm::vec3(1.0f, 0.0f, 0.0f))
				* glm::rotate(rotation, glm::radians(m_sceneAngle.y), glm::vec3(0.0f, 1.0f, 0.0f))
				* glm::rotate(rotation, glm::radians(m_sceneAngle.z), glm::vec3(0.0f, 0.0f, 1.0f));
		}


} }