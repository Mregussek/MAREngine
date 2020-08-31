/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "callbacks.h"
#include "WindowLogs.h"

namespace mar {
	namespace callbacks {


		inline void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
			*window_width = width;
			*window_height = height;
		}

		inline void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
			if (*use_input) {
				*mouse_x = (float)xpos;
				*mouse_y = (float)ypos;
			}
		}

		inline void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
			if (*use_input) {
				*scroll_x = (float)xoffset;
				*scroll_y = (float)yoffset;
			}
		}

		inline void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
			if (*use_input) {
				*clicked_button = button;
				*clicked_action = action;
			}
		}

		void setWindowSize(int* height, int* width) {
			window_width = width;
			window_height = height;
		}

		void setMouse(float* x, float* y) {
			mouse_x = x;
			mouse_y = y;
		}

		void setScroll(float* x, float* y) {
			scroll_x = x;
			scroll_y = y;
		}

		void setMouseButtons(int* button, int* action) {
			clicked_button = button;
			clicked_action = action;
		}

		void setUseInput(const bool* use) {
			use_input = use;
		}

		void setCallbacks(GLFWwindow* wind) {
			window = wind;

			glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
			glfwSetCursorPosCallback(window, mouse_callback);
			glfwSetScrollCallback(window, scroll_callback);

			glfwSetMouseButtonCallback(window, mouse_button_callback);
		}


} }