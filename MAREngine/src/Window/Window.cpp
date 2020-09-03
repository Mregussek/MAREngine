/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Window.h"
#include "WindowLogs.h"
#include "callbacks.h"


namespace mar {
	namespace window {
		

		Window Window::s_window;


		static void windowErrorCallback(int error, const char* description) {
			WINDOW_ERROR("CALLBACKS: GLFW Error: {}", description);
		}


		void Window::initialize(const int& H, const int& W, const char* wN) {
			m_height = H;
			m_width = W;
			m_windowName = wN;

			glfwSetErrorCallback(windowErrorCallback);

			if (!glfwInit()) {
				WINDOW_ERROR("WINDOW: glfwInit() failure!");
				char c = getchar();
				exit(0);
			}

			WINDOW_INFO("WINDOW: GLFW has been initialized successfully!");

			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

			/// TODO: read about fullscreen mode. One of the nullptr's should not be nullptr
			m_window = glfwCreateWindow(m_width, m_height, m_windowName, nullptr, nullptr);
			if (!m_window) {
				glfwTerminate();
				WINDOW_ERROR("WINDOW: glfwCreateWindow() failure!");
				char c = getchar();
				exit(0);
			}

			/// make the associated OpenGL context current
			glfwMakeContextCurrent(m_window);
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			glewExperimental = GL_TRUE;
			if (glewInit() != GLEW_OK) {
				glfwTerminate();
				WINDOW_ERROR("WINDOW: glewInit() failure");
				char c = getchar();
				exit(0);
			}

			WINDOW_INFO("WINDOW: glew has been initialized successfully!");

			callbacks::setWindowSize(&m_height, &m_width);
			callbacks::setMouse(&m_mouseX, &m_mouseY);
			callbacks::setScroll(&m_scrollX, &m_scrollY);
			callbacks::setMouseButtons(&Input::getMouseButton(), &Input::getMouseButton());
			callbacks::setUseInput(&Input::getUseInput());

			callbacks::setCallbacks(m_window);

			WINDOW_INFO("WINDOW: set all callbacks!");
				
			/// Vertical synchronization(VSync) is enabled by using glfwSwapInterval(1); 
			glfwSwapInterval(1);
			/// Enable DEPTH, in other words 3D
			MAR_CORE_GL_FUNC( glEnable(GL_DEPTH_TEST) );
			/// Enable STENCIL, outliner
			MAR_CORE_GL_FUNC( glEnable(GL_STENCIL_TEST) );
			/// Enable loading PNG files and transparency
			MAR_CORE_GL_FUNC( glEnable(GL_BLEND) );
			MAR_CORE_GL_FUNC( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

			WINDOW_INFO("WINDOW: loaded successfully and window is working!");
		}

		void Window::shutdown() {
			glfwTerminate();

			WINDOW_INFO("WINDOW: shutdown!");
		}

		void Window::swapBuffers() {
			WINDOW_TRACE("WINDOW: going to swap buffers and check if user pressed Esc button!");

			glfwPollEvents();
			glfwSwapBuffers(m_window);

			if (Input::isKeyPressed(MAR_KEY_ESCAPE))
				closeWindow();

			WINDOW_INFO("WINDOW: swapped buffers!");
		}

		void Window::clearScreen() {
			MAR_CORE_GL_FUNC( glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, 1.0f) );
			MAR_CORE_GL_FUNC( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) );

			WINDOW_TRACE("WINDOW: cleared screen!");
		}


} }