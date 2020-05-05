/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef WINDOW_H
#define WINDOW_H

#include "../mar.h"
#include "../Renderer/Camera/Camera.h"

namespace mar {

	class Window {
		GLFWwindow* _window;
		int _width;
		int _height;
		char* _windowName;

	public:
		Window(const int& H, const int& W, char* wN, Camera* camera = nullptr);
		~Window() { glfwTerminate(); }

		static void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
			glViewport(0, 0, width, height);
		}

		void swapBuffers() {
			glfwSwapBuffers(_window);
			glfwPollEvents();
		}

		GLFWwindow* getWindow() const { return _window; }
		const bool shouldClose() const { return !glfwWindowShouldClose(_window); }
	};

	namespace callbacks {
		// Inline Variables and Methods, for solving linker problems
		inline GLFWwindow* window; 
		inline Camera* camera;
		inline void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		inline void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		inline void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		void setCallbacks(GLFWwindow* wind, Camera* cam);
		void setCallbacks(GLFWwindow* wind);
	}

} // end mar namespace

#endif // !WINDOW_H