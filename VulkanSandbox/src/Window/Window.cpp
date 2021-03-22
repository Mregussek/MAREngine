

#include "Window.h"


namespace mar {


	static void error_callback(int error, const char* description) {
		fprintf(stderr, "GLFW Error: %s\n", description);
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	

	Window* Window::s_instance{ nullptr };

	Window* Window::Instance() {
		return s_instance;
	}


	void Window::initialize(const char* name, int32_t width, int32_t height) {
		m_name = name;
		m_width = width;
		m_height = height;
		s_instance = this;

		glfwSetErrorCallback(error_callback);

		if (!glfwInit()) { exit(EXIT_FAILURE); }

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_window = glfwCreateWindow(m_width, m_height, m_name, nullptr, nullptr);
		if (!m_window) {
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		glfwSetKeyCallback(m_window, key_callback);
		//glfwMakeContextCurrent(m_window); // No OpenGLContext!!! We use Vulkan

		//glfwSwapInterval(1); // No OpenGLContext!!! We use Vulkan
	}

	void Window::pollEvents() {
		glfwGetFramebufferSize(m_window, &m_width, &m_height);
		//glfwSwapBuffers(m_window);  // No OpenGLContext!!! We use Vulkan
		glfwPollEvents();
	}

	void Window::terminate() const {
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	bool Window::shouldClose() const {
		return !glfwWindowShouldClose(m_window);
	}

	GLFWwindow* Window::getWindow() const {
		return m_window;
	}

	int32_t Window::getWidth() const {
		return m_width;
	}

	int32_t Window::getHeight() const {
		return m_height;
	}

	bool Window::isKeyPressed(int32_t key) const {
		return glfwGetKey(m_window, key) == GLFW_PRESS || glfwGetKey(m_window, key) == GLFW_REPEAT;
	}


}