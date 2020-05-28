/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "GUI.h"

namespace mar {
	GUI::GUI()
		: _translation(glm::vec3(0.0f, 0.0f, 0.0f)),
		_angle(glm::vec3(0.0f, 0.0f, 0.0f)),
		_rendererConnected(false),
		_startupSceneSize(0)
	{}

	void GUI::initialize(Window* window, const char* glsl_version) {
		_window = window;
		_versionGLSL = glsl_version;

		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(_window->getWindow(), true);
		ImGui_ImplOpenGL3_Init(_versionGLSL);

		for (auto& c : _colors) c = 1.0f;
	}

	void GUI::shutdown() {
		if (_rendererConnected) {
			_renderer->disconnectGUI();
			_rendererConnected = false;
		}
		
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void GUI::loadSceneParameters(Scene* scene) {
		for (unsigned int i = 0; i < scene->getShapesNumber(); i++) {
			push(scene->getCenter(i), scene->getAngle(i));
		}

		_startupSceneSize = scene->getShapesNumber();
	}

	void GUI::connectToRenderer(Renderer* renderer) {
		_renderer = renderer;
		_renderer->connectGUI();
		_rendererConnected = true;
	}

	void GUI::push(const glm::vec3& newCenter, const glm::vec3& newAngle) {
		_guiData.centers.push_back(newCenter);
		_guiData.angles.push_back(newAngle);
	}

	void GUI::prepareNewFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		_guiData.translate = getTranslationMatrix();
		_guiData.rotation = getRotationMatrix();
		_guiData.colors[0] = _colors[0];
		_guiData.colors[1] = _colors[1];
		_guiData.colors[2] = _colors[2];
		_guiData.colors[3] = _colors[3];
	}

	void GUI::display() {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("General")) {

				if (_rendererConnected) {
					eventOnScene();

					ImGui::Separator();

					eventOnEachObjectSeperately();
				}
				else
					ImGui::Text("Renderer is not connected!");
				
				ImGui::Separator();

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				
				ImGui::Separator();

				if (ImGui::Button("Exit")) 
					glfwSetWindowShouldClose(_window->getWindow(), true);

				ImGui::EndMenu();
			}

			
			if (ImGui::BeginMenu("Manage Objects")) {
				if (_rendererConnected) {
					addNewObjectToScene();

					ImGui::Separator();

					deleteObjectFromScene();

				}
				else
					ImGui::Text("Renderer is not connected!");

				ImGui::EndMenu();
			}
			
			if (ImGui::BeginMenu("Statistics")) {

				if (_rendererConnected) {
					displayStatistics();
				}
				else {
					ImGui::Text("Renderer is not connected!");
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Exit")) {

				glfwSetWindowShouldClose(_window->getWindow(), true);

				ImGui::EndMenu();
			}
				
			ImGui::EndMainMenuBar();
		}			

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void GUI::eventOnScene() {
		ImGui::Text("Scene");
		ImGui::SliderFloat3("Translation", &_translation.x, -5.0f, 5.0f);
		ImGui::SliderFloat3("Rotation", &_angle.x, -360.0f, 360.0f);
		ImGui::ColorEdit4("color", _colors);
	}

	void GUI::eventOnEachObjectSeperately() {
		ImGui::MenuItem("Objects Menu", "");

		for (unsigned int i = 0; i < _guiData.centers.size(); i++) {
			// Set current variables to sliders
			_pos[0] = _guiData.centers[i].x;
			_pos[1] = _guiData.centers[i].y;
			_pos[2] = _guiData.centers[i].z;
			_ang[0] = _guiData.angles[i].x;
			_ang[1] = _guiData.angles[i].y;
			_ang[2] = _guiData.angles[i].z;

			// Prepare index on GUI
			char int2char[5];
			sprintf_s(int2char, " %d", i);
			
			char shapeIndex[25] = "";
			if(_rendererConnected)
				strcat_s(shapeIndex, _renderer->getObjectName(i).c_str());
			else
				strcat_s(shapeIndex, "Object ");

			strcat_s(shapeIndex, int2char);
			ImGui::Text(shapeIndex);

			// Change center by GUI
			char DragID[15] = "ObjPos ";
			strcat_s(DragID, int2char);
			ImGui::SliderFloat3(DragID, _pos, -5.0f, 5.0f);

			// Change angle by GUI
			char RotID[15] = "ObjRot ";
			strcat_s(RotID, int2char);
			ImGui::SliderFloat3(RotID, _ang, 0.0f, 360.0f);

			// Set new variables to object
			_guiData.centers[i].x = _pos[0];
			_guiData.centers[i].y = _pos[1];
			_guiData.centers[i].z = _pos[2];
			_guiData.angles[i].x = _ang[0];
			_guiData.angles[i].y = _ang[1];
			_guiData.angles[i].z = _ang[2];
		}
	}

	void GUI::addNewObjectToScene() {
		ImGui::Text("Add Object");
		ImGui::Text("Give value for each coordinate, which is in range (-10, 10)");
		ImGui::InputFloat3("Input Center", _inputCenter);

		if (_inputCenter[0] > 10.0f || _inputCenter[0] < -10.0f)
			return;
		if (_inputCenter[1] > 10.0f || _inputCenter[1] < -10.0f)
			return;
		if (_inputCenter[2] > 10.0f || _inputCenter[2] < -10.0f)
			return;

		if (ImGui::Button("Select Pyramid")) {
				if (_guiData.centers.size() == constants::maxObjectsInScene)
					return;

				glm::vec3 center{ _inputCenter[0], _inputCenter[1] , _inputCenter[2] };
				_renderer->guiPush(GUIPushType::PYRAMID, center);
				this->push(center, { 0.0f, 0.0f, 0.0f });
		}
		
		if (ImGui::Button("Select Cube")) {
				if (_guiData.centers.size() == constants::maxObjectsInScene)
					return;

				glm::vec3 center{ _inputCenter[0], _inputCenter[1] , _inputCenter[2] };
				_renderer->guiPush(GUIPushType::CUBE, center);
				this->push(center, { 0.0f, 0.0f, 0.0f });
			
		}

		if (ImGui::Button("Select Surface")) {
				if (_guiData.centers.size() == constants::maxObjectsInScene)
					return;

				glm::vec3 center{ _inputCenter[0], _inputCenter[1] , _inputCenter[2] };
				_renderer->guiPush(GUIPushType::SURFACE, center);
				this->push(center, { 0.0f, 0.0f, 0.0f });
		}

		if (ImGui::Button("Select Wall")) {
				if (_guiData.centers.size() == constants::maxObjectsInScene)
					return;

				glm::vec3 center{ _inputCenter[0], _inputCenter[1] , _inputCenter[2] };
				_renderer->guiPush(GUIPushType::WALL, center);
				this->push(center, { 0.0f, 0.0f, 0.0f });
			
		}
	}

	void GUI::deleteObjectFromScene() {
		ImGui::MenuItem("Delete Object", "");

		for (unsigned int i = _startupSceneSize; i < _guiData.centers.size(); i++) {
			char int2char[5];
			sprintf_s(int2char, " %d ", i);
			char shapeIndex[30] = " Delete ";

			strcat_s(shapeIndex, _renderer->getObjectName(i).c_str());
			strcat_s(shapeIndex, int2char);

			if (ImGui::Button(shapeIndex)) {
				//! here delete object from scene!!!!
				_guiData.centers.erase(_guiData.centers.begin() + i);
				_guiData.angles.erase(_guiData.angles.begin() + i);
			}
		}
	}

	void GUI::displayStatistics() {
		RendererStatistics stats = _renderer->getStatistics();

		char int2char[7];
		sprintf_s(int2char, "%d ", stats._countOfDrawCalls);
		char drawCalls[28] = "Draw Calls: ";
		strcat_s(drawCalls, int2char);
		ImGui::Text(drawCalls);

		char int2char1[7];
		sprintf_s(int2char1, "%d ", stats._countOfShapes);
		char drawCalls1[28] = "Shapes Count: ";
		strcat_s(drawCalls1, int2char1);
		ImGui::Text(drawCalls1);

		char int2char2[7];
		sprintf_s(int2char2, "%d ", stats._countOfVertices);
		char drawCalls2[28] = "Vertices: ";
		strcat_s(drawCalls2, int2char2);
		ImGui::Text(drawCalls2);

		char int2char3[7];
		sprintf_s(int2char3, "%d ", stats._countOfIndices);
		char drawCalls3[28] = "Indices: ";
		strcat_s(drawCalls3, int2char3);
		ImGui::Text(drawCalls3);

		char int2char4[7];
		sprintf_s(int2char4, "%d ", stats._countOfTriangles);
		char drawCalls4[28] = "Triangles: ";
		strcat_s(drawCalls4, int2char4);
		ImGui::Text(drawCalls4);
	}

	const glm::mat4 GUI::getTranslationMatrix() const { 
		return glm::translate(glm::mat4(1.0f), _translation);
	}

	const glm::mat4 GUI::getRotationMatrix() const {
		glm::mat4 rotation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		return glm::rotate(rotation, glm::radians(_angle.x), glm::vec3(1.0f, 0.0f, 0.0f))
			* glm::rotate(rotation, glm::radians(_angle.y), glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::rotate(rotation, glm::radians(_angle.z), glm::vec3(0.0f, 0.0f, 1.0f));
	}
}