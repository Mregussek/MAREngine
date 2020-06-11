/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Window.h"
#include "../Debug/Log.h"


namespace mar {
	namespace window {
    

		void Window::initialize(const int& H, const int& W, const char* wN, graphics::Camera* camera) {
			_height = H;
			_width = W;
			_windowName = wN;

			if (!glfwInit()) {
				MAR_CORE_ERROR("glfw() init failure");
				exit(0);
			}

			MAR_CORE_TRACE("GLFW has been loaded successfully!");

			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

			/// TODO: read about fullscreen mode. One of the nullptr's should not be nullptr
			_window = glfwCreateWindow(_width, _height, _windowName, nullptr, nullptr);
			if (!_window) {
				glfwTerminate();
				MAR_CORE_ERROR("Cannot create window!");
				exit(0);
			}

			/// make the associated OpenGL context current
			glfwMakeContextCurrent(_window);
			glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

#ifdef IMPORT_GLEW
			glewExperimental = GL_TRUE;
			if (glewInit() != GLEW_OK) {
				glfwTerminate();
				MAR_CORE_ERROR("glewInit() failure");
				exit(0);
			}
#else
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				std::cout << "Failed to initialize GLAD" << std::endl;
				exit(0);
			}
#endif

			if (camera == nullptr) {
				callbacks::setCallbacks(_window);

				MAR_CORE_TRACE("Did not add camera callbacks!");
			}
			else {
				callbacks::setCallbacks(_window, camera);

				MAR_CORE_TRACE("Added camera callbacks!");
			}
				

			/// Vertical synchronization(VSync) is enabled by using glfwSwapInterval(1); 
			glfwSwapInterval(1);
			glEnable(GL_DEPTH_TEST);

			MAR_CORE_INFO("OpenGL loaded successfully and window is working!");
		}

		void Window::shutdown() {
			glfwTerminate();

			MAR_CORE_INFO("Terminating GLFW!");
		}

		void Window::swapBuffers() {
			glfwSwapBuffers(_window);
			glfwPollEvents();

			clearScreen();
		}

		void Window::clearScreen() {
			glClearColor(0.22f, 0.69f, 0.87f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		GLFWwindow* Window::getWindow() const {
			return _window;
		}

		const bool Window::shouldClose() const {
			return !glfwWindowShouldClose(_window);
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

			void setCallbacks(GLFWwindow* wind, graphics::Camera* cam) {
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


} }