/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_WINDOW_INPUT_H
#define MAR_ENGINE_WINDOW_INPUT_H


#include "../mar.h"
#include "buttons_def.h"
#include "Window.h"


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

			static void initialize(GLFWwindow* window);

			static bool isKeyPressed(int key);
			static bool isKeyPressed_NotViewport(int key);

			static bool isMousePressed(int key);
			static bool isMousePressed_NotViewport(int key);

			// --- GET METHODS --- //
			static const bool& getUseInput() { return s_useInput; }

			static int& getMouseButton() { return s_clickedButton; }
			static int& getMouseAction() { return s_clickedAction; }

			// --- SET METHODS --- //
			static void enableInput();
			static void disableInput();
		};


} }


#endif // !MAR_ENGINE_WINDOW_INPUT_H

