/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Input.h"

namespace mar {
	namespace window {

		GLFWwindow* Input::s_nativewindow;
		bool Input::s_useInput{ false };

		int Input::s_clickedButton{0};
		int Input::s_clickedAction{0};


} }
