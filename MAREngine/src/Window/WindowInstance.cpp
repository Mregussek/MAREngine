/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#include "WindowInstance.h"


namespace marengine {


	template<typename WindowType>
	bool WindowInstance<WindowType>::initialize(int32_t width, int32_t height, const char* name) {
		return false;
	}

	template<typename WindowType>
	void WindowInstance<WindowType>::endRenderLoop() {

	}

	template<typename WindowType>
	void WindowInstance<WindowType>::terminate() {

	}

	template<typename WindowType>
	void WindowInstance<WindowType>::swapBuffers() {

	}

	template<typename WindowType>
	bool WindowInstance<WindowType>::isGoingToClose() const {
		return true;
	}

	template<typename WindowType>
	void WindowInstance<WindowType>::setVerticalSync(int32_t setter) const {

	}

	template<typename WindowType>
	bool WindowInstance<WindowType>::isKeyPressed(int32_t key) const {
		return false;
	}

	template<typename WindowType>
	bool WindowInstance<WindowType>::isMousePressed(int32_t key) const {
		return false;
	}

	template<typename WindowType>
	float WindowInstance<WindowType>::getMousePositionX() const {
		return 0.f;
	}

	template<typename WindowType>
	float WindowInstance<WindowType>::getMousePositionY() const {
		return 0.f;
	}

	template<typename WindowType>
	float WindowInstance<WindowType>::getScrollX() const {
		return 0.f;
	}

	template<typename WindowType>
	float WindowInstance<WindowType>::getScrollY() const {
		return 0.f;
	}


}
