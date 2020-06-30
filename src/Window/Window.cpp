/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Window.h"
#include "../Debug/Log.h"


namespace mar {
	namespace window {
    

		void Window::initialize(const int& H, const int& W, const char* wN) {
			m_height = H;
			m_width = W;
			m_windowName = wN;

			if (!glfwInit()) {
				MAR_CORE_ERROR("glfw() init failure");
				exit(0);
			}

			MAR_CORE_TRACE("GLFW has been loaded successfully!");

			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

			/// TODO: read about fullscreen mode. One of the nullptr's should not be nullptr
			m_window = glfwCreateWindow(m_width, m_height, m_windowName, nullptr, nullptr);
			if (!m_window) {
				glfwTerminate();
				MAR_CORE_ERROR("Cannot create window!");
				exit(0);
			}

			/// make the associated OpenGL context current
			glfwMakeContextCurrent(m_window);
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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

			callbacks::setWindowSize(&m_height, &m_width);
			callbacks::setMouse(&m_mouseX, &m_mouseY);
			callbacks::setScroll(&m_scrollX, &m_scrollY);
			callbacks::setUseInput(&Input::getUseInput());

			callbacks::setCallbacks(m_window);
				
			/// Vertical synchronization(VSync) is enabled by using glfwSwapInterval(1); 
			glfwSwapInterval(1);
			/// Enable DEPTH, in other words 3D
			glEnable(GL_DEPTH_TEST);
			/// Enable loading PNG files and transparency
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			

			MAR_CORE_INFO("OpenGL loaded successfully and window is working!");
		}

		void Window::shutdown() {
			glfwTerminate();

			MAR_CORE_INFO("Terminating GLFW!");
		}

		void Window::swapBuffers() {
			glfwSwapBuffers(m_window);
			glfwPollEvents();
		}

		void Window::clearScreen() {
			glClearColor(0.22f, 0.69f, 0.87f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glViewport(0, 0, m_width, m_height);

			if (glfwGetKey(m_window, MAR_KEY_ESCAPE) == MAR_KEY_PRESS)
				glfwSetWindowShouldClose(m_window, true);
		}

		namespace callbacks {
			inline void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
				*window_width = width;
				*window_height = height;
			}

			inline void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
				if (*use_input) {
					*mouse_x = (float)xpos;
					*mouse_y = (float)ypos;
				}
			}

			inline void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
				if (*use_input) {
					*scroll_x = (float)xoffset;
					*scroll_y = (float)yoffset;
				}
			}

			void setWindowSize(int* height, int* width) {
				window_width = width;
				window_height = height;
			}

			void setMouse(float* x, float* y) {
				mouse_x = x;
				mouse_y = y;
			}

			void setScroll(float* x, float* y) {
				scroll_x = x;
				scroll_y = y;
			}

			void setUseInput(const bool* use) {
				use_input = use;
			}

			void setCallbacks(GLFWwindow* wind) {
				window = wind;

				glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
				glfwSetCursorPosCallback(window, mouse_callback);
				glfwSetScrollCallback(window, scroll_callback);
			}
		}


} }