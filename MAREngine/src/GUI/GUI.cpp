/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "GUI.h"

namespace mar {
	GUI::GUI(Window* window, const char* glsl_version)
		: _window(window),
		_translation(glm::vec3(0.0f, 0.0f, 0.0f)),
		_angle(glm::vec3(0.0f, 0.0f, 0.0f)),
		_versionGLSL(glsl_version),
		_index(0),
		_rendererConnected(false),
		_checkPyramid(false),
		_checkCube(false),
		_checkSurface(false)
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(_window->getWindow(), true);
		ImGui_ImplOpenGL3_Init(_versionGLSL);

		for (auto& c : _colors) c = 1.0f;
	}

	GUI::~GUI() {
		_rendererConnected = false;
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void GUI::connectToRenderer(Renderer* renderer) {
		_renderer = renderer;
		_rendererConnected = true;
	}

	void GUI::push(const glm::vec3& newCenter, const glm::vec3& newAngle) {
		_centersOfObjects.push_back(newCenter);
		_angles.push_back(newAngle);
	}

	void GUI::prepareNewFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void GUI::display() {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("General")) {

				ImGui::MenuItem("Scene", "");

				ImGui::SliderFloat3("Translation", &_translation.x, -10.0f, 10.0f);
				ImGui::SliderFloat3("Rotation", &_angle.x, -360.0f, 360.0f);
				ImGui::ColorEdit4("color", _colors);
				
				ImGui::Separator();

				displayObjectsGeneralGUI();

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				
				ImGui::Separator();

				if (ImGui::Button("Exit")) 
					glfwSetWindowShouldClose(_window->getWindow(), true);

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Manage Objects")) {

				ImGui::MenuItem("Add Object", "");
				ImGui::Text("Give value for each coordinate, which is in range (-10, 10)");

				ImGui::InputFloat3("Input Center", _inputCenter);

				if (ImGui::Button("Select Pyramid")) {
					if (_rendererConnected) {
						glm::vec3 center{ _inputCenter[0], _inputCenter[1] , _inputCenter[2] };
						_renderer->guiPushPyramid(center);
						this->push(center, { 0.0f, 0.0f, 0.0f });
					}
				}

				if (ImGui::Button("Select Cube")) {
					if (_rendererConnected) {
						glm::vec3 center{ _inputCenter[0], _inputCenter[1] , _inputCenter[2] };
						_renderer->guiPushCube(center);
						this->push(center, { 0.0f, 0.0f, 0.0f });
					}
				}

				if (ImGui::Button("Select Surface")) {
					if (_rendererConnected) {
						glm::vec3 center{ _inputCenter[0], _inputCenter[1] , _inputCenter[2] };
						_renderer->guiPushSurface(center);
						this->push(center, { 0.0f, 0.0f, 0.0f });
					}
				}

				ImGui::Separator();

				ImGui::MenuItem("Delete Object", "");

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}			

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void GUI::displayObjectsGeneralGUI() {
		ImGui::MenuItem("Objects Menu", "");

		for (unsigned int i = 0; i < _centersOfObjects.size(); i++) {
			// Set current variables to sliders
			_pos[0] = _centersOfObjects[i].x;
			_pos[1] = _centersOfObjects[i].y;
			_pos[2] = _centersOfObjects[i].z;
			_ang[0] = _angles[i].x;
			_ang[1] = _angles[i].y;
			_ang[2] = _angles[i].z;

			// Prepare index on GUI
			char int2char[2];
			sprintf_s(int2char, "%d", i);
			char shapeIndex[10] = "Object ";
			strcat_s(shapeIndex, int2char);
			ImGui::Text(shapeIndex);

			// Change center by GUI
			char DragID[10] = "ObjPos ";
			strcat_s(DragID, int2char);
			ImGui::SliderFloat3(DragID, _pos, -10.0f, 10.0f);

			// Change angle by GUI
			char RotID[10] = "ObjRot ";
			strcat_s(RotID, int2char);
			ImGui::SliderFloat3(RotID, _ang, 0.0f, 360.0f);

			// Set new variables to object
			_centersOfObjects[i].x = _pos[0];
			_centersOfObjects[i].y = _pos[1];
			_centersOfObjects[i].z = _pos[2];
			_angles[i].x = _ang[0];
			_angles[i].y = _ang[1];
			_angles[i].z = _ang[2];
		}
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

	const std::vector<glm::vec3>& GUI::getCentersVector() const {
		return _centersOfObjects;
	}

	const std::vector<glm::vec3>& GUI::getAnglesVector() const {
		return _angles;
	}
}