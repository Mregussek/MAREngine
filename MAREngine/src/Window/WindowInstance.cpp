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


	template<typename WindowType>
	bool Window<WindowType>::initialize(int32_t width, int32_t height, const char* name) {
		return false;
	}

	template<typename WindowType>
	void Window<WindowType>::endRenderLoop() {

	}

	template<typename WindowType>
	void Window<WindowType>::terminate() {

	}

	template<typename WindowType>
	void Window<WindowType>::clear() const {

	}

	template<typename WindowType>
	void Window<WindowType>::swapBuffers() {

	}

	template<typename WindowType>
	bool Window<WindowType>::isGoingToClose() const {
		return true;
	}

	template<typename WindowType>
	void Window<WindowType>::setVerticalSync(int32_t setter) const {

	}

	template<typename WindowType>
	bool Window<WindowType>::isKeyPressed(int32_t key) const {
		return false;
	}

	template<typename WindowType>
	bool Window<WindowType>::isMousePressed(int32_t key) const {
		return false;
	}

	template<typename WindowType>
	float Window<WindowType>::getMousePositionX() const {
		return 0.f;
	}

	template<typename WindowType>
	float Window<WindowType>::getMousePositionY() const {
		return 0.f;
	}

	template<typename WindowType>
	float Window<WindowType>::getScrollX() const {
		return 0.f;
	}

	template<typename WindowType>
	float Window<WindowType>::getScrollY() const {
		return 0.f;
	}


}
