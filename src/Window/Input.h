/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_WINDOW_INPUT_H
#define MAR_ENGINE_WINDOW_INPUT_H


#include "../mar.h"
#include "buttons_def.h"
#include "Window.h"
#include "../Debug/Log.h"


namespace mar {
	namespace window {


		class Input {
			static GLFWwindow* s_nativewindow;
			static bool s_useInput;

		public:

			static void initialize(GLFWwindow* window) {
				s_nativewindow = window;

				MAR_CORE_INFO("Native window is set for Input!");
			}

			static bool isKeyPressed(int key) {
				if (s_useInput) {
					return glfwGetKey(s_nativewindow, key) == MAR_KEY_PRESS;
				}
			}

			// --- GET METHODS --- //
			static const bool& getUseInput() { return s_useInput; }

			// --- SET METHODS --- //
			static void enableInput() { s_useInput = true; }
			static void disableInput() { s_useInput = false; }
		};


} }


#endif // !MAR_ENGINE_WINDOW_INPUT_H

