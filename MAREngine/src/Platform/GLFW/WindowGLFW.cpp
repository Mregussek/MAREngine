/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "WindowGLFW.h"


namespace mar {
	namespace platforms {


		bool WindowGLFW::initialize(int32_t height, int32_t width, const char* name) {
			glfwSetErrorCallback(callbacks::windowErrorCallback);

			int32_t glfw_init = glfwInit();

			if (glfw_init != GLFW_TRUE) {
				PLATFORM_ERROR("WINDOW_GLFW: glfwInit() failure!");
				return false;
			}

			PLATFORM_INFO("WINDOW_GLFW: GLFW has been initialized successfully!");

			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

			m_window = glfwCreateWindow(width, height, name, nullptr, nullptr);
			if (!m_window) {
				glfwTerminate();
				PLATFORM_ERROR("WINDOW_GLFW: glfwCreateWindow() failure!");
				return false;
			}

			glfwMakeContextCurrent(m_window);
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			glfwSetFramebufferSizeCallback(m_window, callbacks::framebufferSizeCallback);
			glfwSetCursorPosCallback(m_window, callbacks::cursorPositionCallback);
			glfwSetScrollCallback(m_window, callbacks::scrollCallback);
			glfwSetMouseButtonCallback(m_window, callbacks::mouseButtonCallback);

			PLATFORM_INFO("WINDOW_GLFW: set all callbacks!");

			setVSync(1);

			return true;
		}

		void WindowGLFW::terminate() {
			glfwTerminate();

			PLATFORM_INFO("WINDOW_GLFW: window terminated");
		}

		void WindowGLFW::swapBuffers() {
			glfwPollEvents();
			glfwSwapBuffers(m_window);

			PLATFORM_TRACE("WINDOW_GLFW: window has swapped buffers");
		}

		void WindowGLFW::setVSync(int32_t set) {
			glfwSwapInterval(set);

			PLATFORM_INFO("WINDOW_GLFW: is vertical synchronization used - {}", set);
		}

		bool WindowGLFW::isKeyPressed(int32_t key) {
			return glfwGetKey(m_window, key) == GLFW_PRESS || glfwGetKey(m_window, key) == GLFW_REPEAT;
		}

		bool WindowGLFW::isMousePressed(int32_t key) {
			return glfwGetMouseButton(m_window, key) == GLFW_PRESS || glfwGetMouseButton(m_window, key) == GLFW_REPEAT;
		}

} }