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
#include "../Platform/GLFW/WindowGLFW.h"


namespace mar {
	namespace editor { class GUI; class ProjectSelectionGUI; }

	namespace window {
	
		class Input;


		class Window {
			friend class Input;
			friend class editor::GUI;
			friend class editor::ProjectSelectionGUI;

			static Window* s_instance;

			platforms::WindowGLFW m_window;
			uint32_t m_width{ 0 };
			uint32_t m_height{ 0 };
			maths::vec3 m_background;
			bool m_closeAfterTerminate{ false };

		public:
			static Window& getInstance() { return *s_instance; }
			
			Window() = default;

			void updateBackgroundColor(maths::vec3 new_back) { m_background = new_back; }

			void initialize(int32_t width, int32_t height, const char* name);

			void terminate();

			void clear();

			void update();

			bool isGoingToClose();

			void endRenderLoop();

			void exitApp();
		};


} }


#endif // !MAR_ENGINE_WINDOW_WINDOW_H