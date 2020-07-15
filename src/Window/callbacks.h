/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_WINDOW_CALLBACKS_H
#define MAR_ENGINE_WINDOW_CALLBACKS_H

#include "../mar.h"


namespace mar {
	namespace callbacks {
		// Inline Variables and Methods, for solving linker problems
		static inline GLFWwindow* window;

		static inline int* window_width;
		static inline int* window_height;

		static inline float* mouse_x;
		static inline float* mouse_y;

		static inline float* scroll_x;
		static inline float* scroll_y;

		static inline int* clicked_button;
		static inline int* clicked_action;

		static inline const bool* use_input;

		inline void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		inline void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		inline void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		inline void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		void setWindowSize(int* height, int* width);
		void setMouse(float* x, float* y);
		void setScroll(float* x, float* y);
		void setMouseButtons(int* button, int* action);
		void setUseInput(const bool* use);
		void setCallbacks(GLFWwindow* wind);

	
	
} }


#endif // !MAR_ENGINE_WINDOW_CALLBACKS_H
