/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_PLATFORMS_WINDOW_GLFW_CALLBACKS_H
#define MAR_ENGINE_PLATFORMS_WINDOW_GLFW_CALLBACKS_H


#include "../../mar.h"
#include "../PlatformLogs.h"


namespace mar {
	namespace platforms {
		namespace callbacks {


			inline int32_t window_width{ 0 };
			inline int32_t window_height{ 0 };

			inline double mouse_xpos{ 0.0 };
			inline double mouse_ypos{ 0.0 };

			inline double scroll_x{ 0.0 };
			inline double scroll_y{ 0.0 };

			inline int32_t mouse_button{ 0 };
			inline int32_t mouse_action{ 0 };
			inline int32_t mouse_mods{ 0 };

			static void windowErrorCallback(int32_t error, const char* description) {
				PLATFORM_ERROR("WINDOW_GLFW: Error number: {}, description: {}", error, description);
			}

			static void framebufferSizeCallback(GLFWwindow* window, int32_t w, int32_t h) {
				window_width = w;
				window_height = h;
			}

			static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
				mouse_xpos = xpos;
				mouse_ypos = ypos;
			}

			static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
				scroll_x = xoffset;
				scroll_y = yoffset;
			}

			static void mouseButtonCallback(GLFWwindow* window, int32_t button, int32_t action, int32_t mods) {
				mouse_button = button;
				mouse_action = action;
				mouse_mods = mods;
			}


} } }


#endif // !MAR_ENGINE_PLATFORMS_WINDOW_GLFW_CALLBACKS_H