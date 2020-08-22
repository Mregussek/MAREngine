/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Input.h"
#include "WindowLogs.h"


namespace mar {
	namespace window {

		GLFWwindow* Input::s_nativewindow;
		bool Input::s_useInput{ false };

		int Input::s_clickedButton{0};
		int Input::s_clickedAction{0};


		void Input::initialize(GLFWwindow* window) {
			s_nativewindow = window;

			WINDOW_INFO("INPUT: initialized - native window is set for Input!");
		}

		bool Input::isKeyPressed(int key) {
			WINDOW_TRACE("INPUT: going to check key press - {}, input is {}", key, s_useInput ? "enabled" : "disabled");

			if (s_useInput) {
				return glfwGetKey(s_nativewindow, key) == MAR_KEY_PRESS ||
					glfwGetKey(s_nativewindow, key) == MAR_KEY_REPEAT;
			}
			else {
				return false;
			}
		}

		bool Input::isKeyPressed_NotViewport(int key) {
			return glfwGetKey(s_nativewindow, key) == MAR_KEY_PRESS ||
				glfwGetKey(s_nativewindow, key) == MAR_KEY_REPEAT;
		}

		bool Input::isMousePressed(int key) {
			if (s_useInput) {
				return glfwGetMouseButton(s_nativewindow, key) == MAR_KEY_PRESS ||
					glfwGetMouseButton(s_nativewindow, key) == MAR_KEY_REPEAT;
			}
			else {
				return false;
			}
		}

		bool Input::isMousePressed_NotViewport(int key) {
			return glfwGetMouseButton(s_nativewindow, key) == MAR_KEY_PRESS ||
				glfwGetMouseButton(s_nativewindow, key) == MAR_KEY_REPEAT;
		}

		void Input::enableInput() {
			s_useInput = true;
			WINDOW_INFO("INPUT: enabled!");
		}

		void Input::disableInput() {
			s_useInput = false;
			WINDOW_INFO("INPUT: disabled!");
		}



} }
