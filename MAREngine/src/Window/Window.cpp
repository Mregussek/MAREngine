
#include "Window.h"

Window::Window(const int& H, const int& W, char* wN)
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
	glfwSetFramebufferSizeCallback(_window, Window::frameBuffer_SizeCallback);
	glfwSwapInterval(1);

#ifdef IMPORT_GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		std::cout << "glewInit() failure" << std::endl;
		exit(0);
	}
#else
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(0);
	}
#endif
}