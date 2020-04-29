
#ifndef WINDOW_H
#define WINDOW_H

#include "../mre.h"

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

	void updateInput() {
		glfwPollEvents();
		glfwSwapBuffers(_window);
	}

	GLFWwindow* getWindow() const { return _window; }
	static void frameBuffer_SizeCallback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }
};

#endif // !WINDOW_H