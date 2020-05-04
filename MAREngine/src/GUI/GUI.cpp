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
		_versionGLSL(glsl_version) ,
		_index(0)
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(_window->getWindow(), true);
		ImGui_ImplOpenGL3_Init(_versionGLSL);

		for (auto& c : _colors) c = 1.0f;
	}

	GUI::~GUI() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
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
		{
			ImGui::Begin("MAREngine GUI");

			ImGui::Text("Scene");
			ImGui::SliderFloat3("Translation", &_translation.x, -2.0f, 2.0f);
			ImGui::SliderFloat3("Rotation", &_angle.x, 0.0f, 360.0f);
			ImGui::ColorEdit4("color", _colors);
			ImGui::Text("Objects");

			for (unsigned int i = 0; i < _centersOfObjects.size(); i++) {
				float pos[3];
				float ang[3];

				// Set current variables to sliders
				pos[0] = _centersOfObjects[i].x;
				pos[1] = _centersOfObjects[i].y;
				pos[2] = _centersOfObjects[i].z;
				ang[0] = _angles[i].x;
				ang[1] = _angles[i].y;
				ang[2] = _angles[i].z;

				// Prepare index on GUI
				char int2char[2];
				sprintf_s(int2char, "%d", i);
				char shapeIndex[7] = "Cube ";
				strcat_s(shapeIndex, int2char);
				ImGui::Text(shapeIndex);

				// Change center by GUI
				char DragID[10] = "CubePos ";
				strcat_s(DragID, int2char);
				ImGui::SliderFloat3(DragID, pos, -10.0f, 10.0f);

				// Change angle by GUI
				char RotID[10] = "CubeRot ";
				strcat_s(RotID, int2char);
				ImGui::SliderFloat3(RotID, ang, 0.0f, 360.0f);

				// Set new variables to object
				_centersOfObjects[i].x = pos[0];
				_centersOfObjects[i].y = pos[1];
				_centersOfObjects[i].z = pos[2];
				_angles[i].x = ang[0];
				_angles[i].y = ang[1];
				_angles[i].z = ang[2];
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			if (ImGui::Button("Exit")) glfwSetWindowShouldClose(_window->getWindow(), true);

			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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