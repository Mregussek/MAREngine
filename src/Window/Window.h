/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef WINDOW_H
#define WINDOW_H

#include "../mar.h"
#include "Input.h"
#include "../Debug/Log.h"


namespace mar {
	namespace window {


		class Window {
			static Window s_window;
			GLFWwindow* m_window;
			const char* m_windowName;
			// --- Window Size Callback
			int m_width;
			int m_height;
			// --- Mouse Callback
			float m_mouseX;
			float m_mouseY;
			// --- Scroll Callback
			float m_scrollX;
			float m_scrollY;

		public:
			Window() = default;

			void initialize(const int& H, const int& W, const char* wN);
			void shutdown();

			void swapBuffers();
			void clearScreen();

			static void windowErrorCallback(int error, const char* description) 
			{ MAR_CORE_ERROR("GLFW Error: " + std::string(description)); }
				

			// --- GET METHODS --- //
			inline static Window& getInstance() { return s_window; }
			GLFWwindow* getWindow() const { return m_window; }
			const int& getWidth() const { return m_width; }
			const int& getHeight() const { return m_height; }

			const float& getMouseX() const { return m_mouseX; }
			const float& getMouseY() const { return m_mouseY; }

			const float& getScrollX() const { return m_scrollX; }
			const float& getScrollY() const { return m_scrollY; }

			const bool shouldClose() const { return !glfwWindowShouldClose(m_window); }
		
			// --- SET METHODS --- //
			void closeWindow() { glfwSetWindowShouldClose(m_window, true); }
		};

		namespace callbacks {
			// Inline Variables and Methods, for solving linker problems
			inline GLFWwindow* window;

			inline int* window_width;
			inline int* window_height;

			inline float* mouse_x;
			inline float* mouse_y;

			inline float* scroll_x;
			inline float* scroll_y;

			inline int* clicked_button;
			inline int* clicked_action;

			inline const bool* use_input;

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
		}


} }

#endif // !WINDOW_H