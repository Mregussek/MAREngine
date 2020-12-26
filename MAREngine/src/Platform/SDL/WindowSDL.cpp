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


#include "../../Window/WindowInstance.h"
#include "../OpenGL/SetupOpenGL.h"
#include "ContextSDL.h"


namespace marengine {


	template<>
	bool WindowInstance<SDL_Window>::initialize(int32_t width, int32_t height, const char* name) {
		s_instance = this;

		const int32_t isSDL_OK = SDL_Init(SDL_INIT_VIDEO);
		if (isSDL_OK == SDL_TRUE) {
			MAR_CORE_ERROR("WINDOW_SDL: SDL_Init(SDL_INIT_VIDEO) failed!");
			const char c = getchar();
			return false;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

		m_window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);
		const bool isContextSDL_OK = ContextSDL::create(m_window);
		if (!isContextSDL_OK) {
			MAR_CORE_ERROR("WINDOW_SDL: Cannot initialize Context SDL!");
			const char c = getchar();
			return false;
		}

		setVerticalSync(1);

		const bool isOpenGL_OK = SetupOpenGL::init();
		if (!isOpenGL_OK) {
			MAR_CORE_ERROR("MARENGINE: Cannot initialize OpenGL!");
			const char c = getchar();
			return false;
		}

		ContextSDL::windowGoingToClose = false;

		return true;
	}

	template<>
	void WindowInstance<SDL_Window>::endRenderLoop() {
		ContextSDL::windowGoingToClose = true;
	}

	template<>
	void WindowInstance<SDL_Window>::terminate() {
		ContextSDL::destroy();
		SDL_DestroyWindow(m_window);
		SDL_Quit();

		PLATFORM_INFO("WINDOW_SDL: window terminated");
	}

	template<>
	void WindowInstance<SDL_Window>::setVerticalSync(int32_t setter) const {
		SDL_GL_SetSwapInterval(setter);

		PLATFORM_INFO("WINDOW_SDL: is vertical synchronization used - {}", setter);
	}

	template<>
	bool WindowInstance<SDL_Window>::isGoingToClose() const {
		return ContextSDL::windowGoingToClose;
	}

	template<>
	void WindowInstance<SDL_Window>::swapBuffers() {
		SDL_GL_SwapWindow(m_window);

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				ContextSDL::windowGoingToClose = true;
			}
		}

		PLATFORM_TRACE("WINDOW_SDL: window has swapped buffers");
	}

	template<>
	bool WindowInstance<SDL_Window>::isKeyPressed(int32_t key) const {
		return false;
	}

	template<>
	bool WindowInstance<SDL_Window>::isMousePressed(int32_t key) const {
		return false;
	}

	template<>
	float WindowInstance<SDL_Window>::getMousePositionX() const {
		return 0.f;// (float)callbacks::mouse_xpos;
	}

	template<>
	float WindowInstance<SDL_Window>::getMousePositionY() const {
		return 0.f;//(float)callbacks::mouse_ypos;
	}

	template<>
	float WindowInstance<SDL_Window>::getScrollX() const {
		return 0.f;//(float)callbacks::scroll_x;
	}

	template<>
	float WindowInstance<SDL_Window>::getScrollY() const {
		return 0.f;//(float)callbacks::scroll_y;
	}
}
