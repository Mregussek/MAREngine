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


#include "WindowGLFW.h"


namespace mar::platforms {


	bool WindowGLFW::initialize(int32_t height, int32_t width, const char* name) {
		glfwSetErrorCallback(callbacks::windowErrorCallback);

		int32_t glfw_init = glfwInit();

		if (glfw_init != GLFW_TRUE) {
			PLATFORM_ERROR("WINDOW_GLFW: glfwInit() failure!");
			return false;
		}

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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


}
