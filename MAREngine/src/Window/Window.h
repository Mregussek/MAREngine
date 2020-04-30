
#ifndef WINDOW_H
#define WINDOW_H

#include "../mar.h"
#include "SerialPortMonitor.h"

class Window {
	GLFWwindow* _window;
	int _height;
	int _width;
	char* _windowName;

public:
	Window(const int& H, const int& W, char* wN);
	~Window() { glfwTerminate(); }

	void processInput() {
		if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(_window, true);
	}

	void processInput(SerialPortMonitor& spm) {
		if (spm.isConnected()) {
			std::cout << "x: " << spm.getX() << std::endl;
			std::cout << "y: " << spm.getY() << std::endl;
			std::cout << "z: " << spm.getZ() << std::endl;
		}
		else processInput();

		if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(_window, true);
	}

	void swapBuffers() {
		glfwPollEvents();
		glfwSwapBuffers(_window);
	}

	GLFWwindow* getWindow() const { return _window; }
	static void frameBuffer_SizeCallback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }
};

#endif // !WINDOW_H