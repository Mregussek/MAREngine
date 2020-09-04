/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_WINDOW_INPUT_H
#define MAR_ENGINE_WINDOW_INPUT_H


#include "../mar.h"
#include "Window.h"
#include "WindowLogs.h"
#include "buttons_def.h"


namespace mar {
	namespace window {


		class Input {
		public:
			static bool isKeyPressed(int32_t key) {
				return Window::getInstance().m_window.isKeyPressed(key);
			}

			static bool isMousePressed(int32_t key) {
				return Window::getInstance().m_window.isMousePressed(key);
			}

			static float getMousePositionX() {
				return (float)platforms::callbacks::mouse_xpos;
			}

			static float getMousePositionY() {
				return (float)platforms::callbacks::mouse_ypos;
			}

			static float getScrollX() {
				return (float)platforms::callbacks::scroll_x;
			}

			static float getScrollY() {
				return (float)platforms::callbacks::scroll_y;
			}
		};



} }


#endif // !MAR_ENGINE_WINDOW_INPUT_H