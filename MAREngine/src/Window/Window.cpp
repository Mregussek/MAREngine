/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Window.h"

namespace mar {

	Window::Window(const int& H, const int& W, char* wN, Camera* camera)
		: _height(H),
		_width(W),
		_windowName(wN)
	{
		if (!glfwInit()) {
			std::cout << "glfw() init failure" << std::endl;
			exit(0);
		}

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		_window = glfwCreateWindow(_width, _height, _windowName, nullptr, nullptr);
		if (!_window) {
			glfwTerminate();
			std::cout << "glfwCreate() failure" << std::endl;
			exit(0);
		}

		glfwMakeContextCurrent(_window);
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

#ifdef IMPORT_GLEW
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			glfwTerminate();
			std::cout << "glewInit() failure" << std::endl;
			exit(0);
		}
#else
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			exit(0);
		}
#endif
	
		if (camera == nullptr)
			callbacks::setCallbacks(_window);
		else
			callbacks::setCallbacks(_window, camera);
	}
	
	namespace callbacks {
		inline void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
			glViewport(0, 0, width, height);
		}

		inline void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
			if (camera == nullptr) return;

			camera->mouseCallback((float)xpos, (float)ypos);
		}

		inline void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
			if (camera == nullptr) return;

			camera->scrollCallback((float)yoffset);
		}

		void setCallbacks(GLFWwindow* wind, Camera* cam) {
			window = wind;
			camera = cam;

			glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
			glfwSetCursorPosCallback(window, mouse_callback);
			glfwSetScrollCallback(window, scroll_callback);
		}

		void setCallbacks(GLFWwindow* wind) {
			window = wind;
			camera = nullptr;

			glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		}
	}
}