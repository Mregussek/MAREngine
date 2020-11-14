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


#ifndef MAR_ENGINE_WINDOW_WINDOW_H
#define MAR_ENGINE_WINDOW_WINDOW_H


#include "../mar.h"
#include "WindowInstance.h"
#include "buttons_def.h"


namespace mar::window {


	inline bool initialize(int32_t width, int32_t height, const char* name) {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			return window::Window<GLFWwindow>::Instance().initialize(width, height, name);
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			return window::Window<SDL_Window>::Instance().initialize(width, height, name);
		}
		else {
			return false;
		}
	}

	inline void endRenderLoop() {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) { 
			window::Window<GLFWwindow>::Instance().endRenderLoop(); 
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) { 
			window::Window<SDL_Window>::Instance().endRenderLoop(); 
		}
		else {

		}
	}

	inline void terminate() {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			window::Window<GLFWwindow>::Instance().terminate();
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			window::Window<SDL_Window>::Instance().terminate();
		}
		else {

		}
	}

	inline bool isGoingToClose() {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			return window::Window<GLFWwindow>::Instance().isGoingToClose();
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			return window::Window<SDL_Window>::Instance().isGoingToClose();
		}
		else {

		}
	}

	inline void setVerticalSync(int32_t setter) {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			window::Window<GLFWwindow>::Instance().setVerticalSync(setter);
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			window::Window<SDL_Window>::Instance().setVerticalSync(setter);
		}
		else {

		}
	}

	inline void swapBuffers() {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			window::Window<GLFWwindow>::Instance().swapBuffers();
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			window::Window<SDL_Window>::Instance().swapBuffers();
		}
		else {

		}
	}

	inline void clear() {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			window::Window<GLFWwindow>::Instance().clear();
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			window::Window<SDL_Window>::Instance().clear();
		}
		else {

		}
	}

	inline bool isKeyPressed(int32_t key) {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			return window::Window<GLFWwindow>::Instance().isKeyPressed(key);
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			return window::Window<SDL_Window>::Instance().isKeyPressed(key);
		}
		else {
			return false;
		}
	}

	inline bool isMousePressed(int32_t key) {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			return window::Window<GLFWwindow>::Instance().isMousePressed(key);
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			return window::Window<SDL_Window>::Instance().isMousePressed(key);
		}
		else {
			return false;
		}
	}

	inline float getMousePositionX() {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			return window::Window<GLFWwindow>::Instance().getMousePositionX();
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			return window::Window<SDL_Window>::Instance().getMousePositionX();
		}
		else {
			return 0.f;
		}
	}

	inline float getMousePositionY() {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			return window::Window<GLFWwindow>::Instance().getMousePositionY();
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			return window::Window<SDL_Window>::Instance().getMousePositionY();
		}
		else {
			return 0.f;
		}
	}

	inline float getScrollX() {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			return window::Window<GLFWwindow>::Instance().getScrollX();
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			return window::Window<SDL_Window>::Instance().getScrollX();
		}
		else {
			return 0.f;
		}
	}

	inline float getScrollY() {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			return window::Window<GLFWwindow>::Instance().getScrollY();
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			return window::Window<SDL_Window>::Instance().getScrollY();
		}
		else {
			return 0.f;
		}
	}


}


#endif // !MAR_ENGINE_WINDOW_WINDOW_H
