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


#include "Input.h"
#include "Window.h"
#include "WindowLogs.h"


namespace mar {
	namespace window {


		bool Input::isKeyPressed(int32_t key) {
			return Window::getInstance().m_window.isKeyPressed(key);
		}

		bool Input::isMousePressed(int32_t key) {
			return Window::getInstance().m_window.isMousePressed(key);
		}

		float Input::getMousePositionX() {
			return (float)platforms::callbacks::mouse_xpos;
		}

		float Input::getMousePositionY() {
			return (float)platforms::callbacks::mouse_ypos;
		}

		float Input::getScrollX() {
			return (float)platforms::callbacks::scroll_x;
		}

		float Input::getScrollY() {
			return (float)platforms::callbacks::scroll_y;
		}

} }
