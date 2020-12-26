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


namespace marengine {


	class Window {
	public:

		template<typename WindowType>
		static WindowInstance<WindowType> createWindow() {
			if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
				return WindowInstance<GLFWwindow>{};
			}
			else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
				return WindowInstance<SDL_Window>{};
			}
			else {
				return WindowInstance<GLFWwindow>{};
			}
		}

		static void endRenderLoop();

		static void terminate();

		static bool isGoingToClose();

		static void setVerticalSync(int32_t setter);

		static void swapBuffers();

		static void imguiInit();

		static void imguiTerminate();

		static void imguiNewFrame();

		static int32_t getSizeX();

		static int32_t getSizeY();

		static bool isKeyPressed(int32_t key);

		static bool isMousePressed(int32_t key);

		static float getMousePositionX();

		static float getMousePositionY();

		static float getScrollX();

		static float getScrollY();

	};
	


}


#endif // !MAR_ENGINE_WINDOW_WINDOW_H
