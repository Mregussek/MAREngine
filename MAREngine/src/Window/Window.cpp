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
#include "../Platform/SDL/ContextSDL.h"
#include "WindowInstance.h"


namespace mar::window {


	void Window::endRenderLoop() {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			WindowInstance<GLFWwindow>::Instance().endRenderLoop();
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			WindowInstance<SDL_Window>::Instance().endRenderLoop();
		}
		else {

		}
	}

	void Window::terminate() {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			WindowInstance<GLFWwindow>::Instance().terminate();
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			WindowInstance<SDL_Window>::Instance().terminate();
		}
		else {

		}
	}

	bool Window::isGoingToClose() {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			return WindowInstance<GLFWwindow>::Instance().isGoingToClose();
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			return WindowInstance<SDL_Window>::Instance().isGoingToClose();
		}
		else {

		}
	}

	void Window::setVerticalSync(int32_t setter) {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			WindowInstance<GLFWwindow>::Instance().setVerticalSync(setter);
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			WindowInstance<SDL_Window>::Instance().setVerticalSync(setter);
		}
		else {

		}
	}

	void Window::swapBuffers() {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			WindowInstance<GLFWwindow>::Instance().swapBuffers();
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			WindowInstance<SDL_Window>::Instance().swapBuffers();
		}
		else {

		}
	}

	void Window::imguiInit() {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			ImGui_ImplGlfw_InitForOpenGL(WindowInstance<GLFWwindow>::Instance().m_window, true);
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			ImGui_ImplSDL2_InitForOpenGL(WindowInstance<SDL_Window>::Instance().m_window, platforms::ContextSDL::getContext());
		}
		else {

		}
	}

	void Window::imguiTerminate() {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			ImGui_ImplGlfw_Shutdown();
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			ImGui_ImplSDL2_Shutdown();
		}
		else {

		}
	}

	void Window::imguiNewFrame() {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			ImGui_ImplGlfw_NewFrame();
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			ImGui_ImplSDL2_NewFrame(WindowInstance<SDL_Window>::Instance().m_window);
		}
		else {

		}
	}

	bool Window::isKeyPressed(int32_t key) {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			return WindowInstance<GLFWwindow>::Instance().isKeyPressed(key);
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			return WindowInstance<SDL_Window>::Instance().isKeyPressed(key);
		}
		else {
			return false;
		}
	}

	bool Window::isMousePressed(int32_t key) {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			return WindowInstance<GLFWwindow>::Instance().isMousePressed(key);
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			return WindowInstance<SDL_Window>::Instance().isMousePressed(key);
		}
		else {
			return false;
		}
	}

	float Window::getMousePositionX() {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			return WindowInstance<GLFWwindow>::Instance().getMousePositionX();
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			return WindowInstance<SDL_Window>::Instance().getMousePositionX();
		}
		else {
			return 0.f;
		}
	}

	float Window::getMousePositionY() {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			return WindowInstance<GLFWwindow>::Instance().getMousePositionY();
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			return WindowInstance<SDL_Window>::Instance().getMousePositionY();
		}
		else {
			return 0.f;
		}
	}

	float Window::getScrollX() {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			return WindowInstance<GLFWwindow>::Instance().getScrollX();
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			return WindowInstance<SDL_Window>::Instance().getScrollX();
		}
		else {
			return 0.f;
		}
	}

	float Window::getScrollY() {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			return WindowInstance<GLFWwindow>::Instance().getScrollY();
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			return WindowInstance<SDL_Window>::Instance().getScrollY();
		}
		else {
			return 0.f;
		}
	}

}
