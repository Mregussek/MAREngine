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


#ifndef MAR_ENGINE_WINDOW_WINDOW_INSTANCE_H
#define MAR_ENGINE_WINDOW_WINDOW_INSTANCE_H


#include "../mar.h"


namespace marengine {
	

	template<typename WindowType>
	class WindowInstance {

		friend class Window;

	public:

		static WindowInstance<WindowType>& Instance() { return *s_instance; }
		
		WindowInstance<WindowType>() = default;

		bool initialize(int32_t width, int32_t height, const char* name);
		void endRenderLoop();
		void terminate();

		void setVerticalSync(int32_t setter) const;

		bool isGoingToClose() const;

		void swapBuffers();

		bool isKeyPressed(int32_t key) const;
		bool isMousePressed(int32_t key) const;

		float getMousePositionX() const;
		float getMousePositionY() const;

		float getScrollX() const;
		float getScrollY() const;

	private:

		static WindowInstance<WindowType>* s_instance;

		WindowType* m_window;
		
		uint32_t m_width{ 0 };
		uint32_t m_height{ 0 };

	};

	template<typename WindowType>
	WindowInstance<WindowType>* WindowInstance<WindowType>::s_instance{ nullptr };


}


#endif // !MAR_ENGINE_WINDOW_WINDOW_INSTANCE_H