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

		class Window;

		class Input {
			// --- Pointer to native window
			static GLFWwindow* s_nativewindow;
			// --- Mouse Button Callback
			static int s_clickedButton;
			static int s_clickedAction;
			// --- Run-time Attributes
			static bool s_useInput;

		public:

			static void initialize(GLFWwindow* window) {
				s_nativewindow = window;

				MAR_CORE_INFO("Native window is set for Input!");
			}

			static bool isKeyPressed(int key) {
				if (s_useInput) {
					return glfwGetKey(s_nativewindow, key) == MAR_KEY_PRESS ||
						glfwGetKey(s_nativewindow, key) == MAR_KEY_REPEAT;
				}
				else return false;
			}

			static bool isMousePressed(int key) {
				if (s_useInput) {
					return glfwGetMouseButton(s_nativewindow, key) == MAR_KEY_PRESS ||
						glfwGetMouseButton(s_nativewindow, key) == MAR_KEY_REPEAT;
				}
				else return false;
			}

			// --- GET METHODS --- //
			static const bool& getUseInput() { return s_useInput; }

			static int& getMouseButton() { return s_clickedButton; }
			static int& getMouseAction() { return s_clickedAction; }

			// --- SET METHODS --- //
			static void enableInput() { 
				s_useInput = true; 
				MAR_CORE_INFO("INPUT: enabled!");
			}
			static void disableInput() { 
				s_useInput = false; 
				MAR_CORE_INFO("INPUT: disabled!");
			}
		};


} }


#endif // !MAR_ENGINE_WINDOW_INPUT_H

