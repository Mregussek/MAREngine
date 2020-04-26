#include "mre.h"

void frameBufferResizeCallback(GLFWwindow* window, int fbW, int fbH) {
	glViewport(0, 0, fbW, fbH);
}

int main(int argc, char** argv) {
	const char* name = "MREngine";
	const int width{ 640 };
	const int height{ 480 };
	int frameBufferWidth{ 0 };
	int frameBufferHeight{ 0 };

	// init glfw
	glfwInit();

	// some init setup for opengl
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// create window
	GLFWwindow* window{ glfwCreateWindow(width, height, name, nullptr, nullptr) };
	glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);

	glfwMakeContextCurrent(window); // important!!!

	// init glew (needs window and opengl context)
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << argv[0] << " failed!" << std::endl;
		glfwTerminate();
	}

	// main loop
	while (!glfwWindowShouldClose(window)) {
		// update input
		glfwPollEvents();

		// update && draw

		// clear
		glClearColor(0.f, 0.f, 0.f, 1.f); // alpha argument is about transparency
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 

		// draw

		// end draw
		glfwSwapBuffers(window);
		glFlush();
	}

	// end of program
	glfwTerminate(); // free the memory of the window


	return 0;
}