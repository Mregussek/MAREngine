#include "GUI.h"

namespace mar {
	GUI::GUI(Window* window, const char* glsl_version)
		: _window(window),
		_translation(glm::vec3(0.0f, 0.0f, 0.0f)),
		_versionGLSL(glsl_version) 
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(_window->getWindow(), true);
		ImGui_ImplOpenGL3_Init(_versionGLSL);
	}

	GUI::~GUI() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void GUI::prepareNewFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void GUI::display() {
		{
			ImGui::Begin("MAREngine GUI");
			ImGui::SliderFloat3("Translation", &_translation.x, -2.0f, 2.0f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}