/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef WINDOW_H
#define WINDOW_H

#include "../mar.h"
#include "../Debug/Log.h"
#include "Input.h"
#include "callbacks.h"


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


} }

#endif // !WINDOW_H