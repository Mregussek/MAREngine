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


#include "Window.h"
#include "../Platform/OpenGL/SetupOpenGL.h"
#include "WindowLogs.h"


namespace mar::window {

	Window* Window::s_instance{ nullptr };


	void Window::initialize(int32_t width, int32_t height, const char* name) {
		s_instance = this;

		m_width = width;
		m_height = height;

		const bool isGLFW_OK = m_window.initialize(m_height, m_width, name);
		if (!isGLFW_OK) {
			WINDOW_ERROR("WINDOW: Cannot initialize GLFW window!");
			const char c = getchar();
			exit(0);
		}

		WINDOW_INFO("WINDOW: initialized window!");
	
		const bool isOpenGL_OK = platforms::SetupOpenGL::init();
		if (!isOpenGL_OK) {
			MAR_CORE_ERROR("MARENGINE: Cannot initialize OpenGL!");
			const char c = getchar();
			exit(0);
		}

		MAR_CORE_INFO("WINDOW: initialized OpenGL!");
	}

	void Window::terminate() {
		platforms::WindowGLFW::terminate();

		WINDOW_INFO("WINDOW: closed Window!");

		if (m_closeAfterTerminate) {
			exit(0);
		}
	}

	void Window::updateBackgroundColor(maths::vec3 new_back) {
		m_background = new_back; 
	}

	void Window::clear() const {
		platforms::SetupOpenGL::clearScreen(m_background);

		WINDOW_TRACE("WINDOW: is clearing screen");
	}

	void Window::update() {
		m_window.swapBuffers();

		WINDOW_TRACE("WINDOW: updating buffers and callbacks");
	}

	bool Window::isGoingToClose() const {
		return m_window.isGoingToClose();
	}

	void Window::endRenderLoop() {
		m_window.close();

		WINDOW_INFO("WINDOW: ending render loop");
	}

	void Window::exitApp() {
		m_window.close();
		m_closeAfterTerminate = true;

		WINDOW_INFO("WINDOW: exiting application after this frame!")
	}


}
