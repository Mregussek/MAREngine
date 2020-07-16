/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Window.h"
#include "../Debug/Log.h"


namespace mar {
	namespace window {
		

		Window Window::s_window;


		void Window::initialize(const int& H, const int& W, const char* wN) {
			m_height = H;
			m_width = W;
			m_windowName = wN;

			glfwSetErrorCallback(windowErrorCallback);

			if (!glfwInit()) {
				MAR_CORE_ERROR("OPENGL: glfw() init failure");
				exit(0);
			}

			MAR_CORE_TRACE("OPENGL: GLFW has been loaded successfully!");

			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
			glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

			/// TODO: read about fullscreen mode. One of the nullptr's should not be nullptr
			m_window = glfwCreateWindow(m_width, m_height, m_windowName, nullptr, nullptr);
			if (!m_window) {
				glfwTerminate();
				MAR_CORE_ERROR("OPENGL: Cannot create window!");
				exit(0);
			}

			/// make the associated OpenGL context current
			glfwMakeContextCurrent(m_window);
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			glewExperimental = GL_TRUE;
			if (glewInit() != GLEW_OK) {
				glfwTerminate();
				MAR_CORE_ERROR("OPENGL: glewInit() failure");
				exit(0);
			}

			MAR_CORE_TRACE("OPENGL: Glew has been loaded successfully!");

			callbacks::setWindowSize(&m_height, &m_width);
			callbacks::setMouse(&m_mouseX, &m_mouseY);
			callbacks::setScroll(&m_scrollX, &m_scrollY);
			callbacks::setMouseButtons(&Input::getMouseButton(), &Input::getMouseButton());
			callbacks::setUseInput(&Input::getUseInput());

			callbacks::setCallbacks(m_window);
				
			/// Vertical synchronization(VSync) is enabled by using glfwSwapInterval(1); 
			glfwSwapInterval(1);
			/// Enable DEPTH, in other words 3D
			MAR_CORE_GL_FUNC( glEnable(GL_DEPTH_TEST) );
			/// Enable loading PNG files and transparency
			MAR_CORE_GL_FUNC( glEnable(GL_BLEND) );
			MAR_CORE_GL_FUNC( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

			MAR_CORE_INFO("OPENGL: loaded successfully and window is working!");
		}

		void Window::shutdown() {
			glfwTerminate();

			MAR_CORE_INFO("OPENGL: Terminating GLFW!");
		}

		void Window::swapBuffers() {
			glfwSwapBuffers(m_window);
			glfwPollEvents();

			if (Input::isKeyPressed(MAR_KEY_ESCAPE))
				closeWindow();
		}

		void Window::clearScreen() {
			MAR_CORE_GL_FUNC( glClearColor(0.22f, 0.69f, 0.87f, 1.0f) );
			MAR_CORE_GL_FUNC( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
		}


} }