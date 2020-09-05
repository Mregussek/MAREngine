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


#ifndef MAR_ENGINE_PLATFORMS_GLFW_WINDOW_GLFW_H
#define MAR_ENGINE_PLATFORMS_GLFW_WINDOW_GLFW_H


#include "../../mar.h"
#include "WindowCallbacks.h"
#include "../PlatformLogs.h"


namespace mar {
	namespace editor { class GUI; }

	namespace platforms {


		class WindowGLFW {
			friend class editor::GUI;

			GLFWwindow* m_window;

		public:
			bool initialize(int32_t height, int32_t width, const char* name);
			void terminate();

			bool isGoingToClose() { return glfwWindowShouldClose(m_window); }
			void close() { glfwSetWindowShouldClose(m_window, true); }

			void swapBuffers();

			void setVSync(int32_t set);

			bool isKeyPressed(int32_t key);
			bool isMousePressed(int32_t key);
		};


	}
}


#endif // !MAR_ENGINE_PLATFORMS_GLFW_WINDOW_GLFW_H
