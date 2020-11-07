
#ifndef MAR_ENGINE_WINDOW_VULKAN_H
#define MAR_ENGINE_WINDOW_VULKAN_H


#include "../VulkanInclude.h"


static void error_callback(int error, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}


class Window {
public:

	void initialize(const char* name, int32_t width, int32_t height) {
		m_name = name;
		m_width = width;
		m_height = height;

		glfwSetErrorCallback(error_callback);

		if (!glfwInit()) { exit(EXIT_FAILURE); }

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		m_window = glfwCreateWindow(m_width, m_height, m_name, nullptr, nullptr);
		if (!m_window) {
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		glfwSetKeyCallback(m_window, key_callback);
		glfwMakeContextCurrent(m_window);

		glfwSwapInterval(1);
	}

	void clear() {
		glfwGetFramebufferSize(m_window, &m_width, &m_height);
		glViewport(0, 0, m_width, m_height);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void pollEvents() const {
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	void terminate() const {
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
	
	bool shouldClose() const {
		return !glfwWindowShouldClose(m_window);
	}

	GLFWwindow* getWindow() const { 
		return m_window; 
	}

	int32_t getWidth() const {
		return m_width;
	}

	int32_t getHeight() const { 
		return m_height;
	}

private:

	const char* m_name{ "Vulkan Window" };
	int32_t m_width{ 1200 };
	int32_t m_height{ 800 };
	GLFWwindow* m_window{ nullptr };

};



#endif // !MAR_ENGINE_WINDOW_VULKAN_H
