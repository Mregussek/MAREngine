/**
 *           MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


#include "WindowCallbacks.h"
#include "../../Window/WindowInstance.h"
#include "../OpenGL/SetupOpenGL.h"


namespace marengine {


	template<>
	bool WindowInstance<GLFWwindow>::initialize(int32_t width, int32_t height, const char* name) {
		s_instance = this;

		glfwSetErrorCallback(callbacks::windowErrorCallback);

		const int32_t glfw_init = glfwInit();

		if (glfw_init != GLFW_TRUE) {
			PLATFORM_ERROR("WINDOW_GLFW: glfwInit() failure!");
			return false;
		}

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

		PLATFORM_INFO("WINDOW_GLFW: GLFW has been initialized successfully!");

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

		setVerticalSync(1);

		const bool isOpenGL_OK = SetupOpenGL::init();
		if (!isOpenGL_OK) {
			MAR_CORE_ERROR("MARENGINE: Cannot initialize OpenGL!");
			const char c = getchar();
			return false;
		}

		return true;
	}

	template<>
	void WindowInstance<GLFWwindow>::endRenderLoop() {
		glfwSetWindowShouldClose(m_window, true);
	}

	template<>
	void WindowInstance<GLFWwindow>::terminate() {
		glfwTerminate();

		PLATFORM_INFO("WINDOW_GLFW: window terminated");
	}

	template<>
	void WindowInstance<GLFWwindow>::setVerticalSync(int32_t setter) const {
		glfwSwapInterval(setter);

		PLATFORM_INFO("WINDOW_GLFW: is vertical synchronization used - {}", setter);
	}

	template<>
	bool WindowInstance<GLFWwindow>::isGoingToClose() const {
		return glfwWindowShouldClose(m_window); 
	}

	template<>
	void WindowInstance<GLFWwindow>::swapBuffers() {
		m_width = callbacks::window_width;
		m_height = callbacks::window_height;

		glfwPollEvents();
		glfwSwapBuffers(m_window);

		PLATFORM_TRACE("WINDOW_GLFW: window has swapped buffers");
	}

	template<>
	bool WindowInstance<GLFWwindow>::isKeyPressed(int32_t key) const {
		return glfwGetKey(m_window, key) == GLFW_PRESS || glfwGetKey(m_window, key) == GLFW_REPEAT;
	}

	template<>
	bool WindowInstance<GLFWwindow>::isMousePressed(int32_t key) const {
		return glfwGetMouseButton(m_window, key) == GLFW_PRESS || glfwGetMouseButton(m_window, key) == GLFW_REPEAT;
	}

	template<>
	float WindowInstance<GLFWwindow>::getMousePositionX() const {
		return (float)callbacks::mouse_xpos;
	}

	template<>
	float WindowInstance<GLFWwindow>::getMousePositionY() const {
		return (float)callbacks::mouse_ypos;
	}

	template<>
	float WindowInstance<GLFWwindow>::getScrollX() const {
		return (float)callbacks::scroll_x;
	}

	template<>
	float WindowInstance<GLFWwindow>::getScrollY() const {
		return (float)callbacks::scroll_y;
	}


}
