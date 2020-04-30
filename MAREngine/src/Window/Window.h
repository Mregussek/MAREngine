
#ifndef WINDOW_H
#define WINDOW_H

#include "../mar.h"
#include "../Camera/Camera.h"

namespace mar {
	class Window {
		GLFWwindow* _window;
		int _height;
		int _width;
		char* _windowName;

	public:
		Window(const int& H, const int& W, char* wN);
		~Window() { glfwTerminate(); }

		static void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
			glViewport(0, 0, width, height);
		}

		void swapBuffers() {
			glfwSwapBuffers(_window);
			glfwPollEvents();
		}

		GLFWwindow* getWindow() const { return _window; }
	};
}

#endif // !WINDOW_H