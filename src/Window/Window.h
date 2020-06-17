/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef WINDOW_H
#define WINDOW_H

#include "../mar.h"
#include "../Renderer/Camera/Camera.h"

namespace mar {
	namespace window {


		class Window {
			GLFWwindow* m_window;
			int m_width;
			int m_height;
			const char* m_windowName;

		public:
			Window() = default;

			void initialize(const int& H, const int& W, const char* wN, graphics::Camera* camera = nullptr);
			void shutdown();

			void swapBuffers();
			void clearScreen();

			// --- GET METHODS
			GLFWwindow* getWindow() const { return m_window; }
			const int& getWidth() const { return m_width; }
			const int& getHeight() const { return m_height; }

			const bool shouldClose() const { return !glfwWindowShouldClose(m_window); }
		};

		namespace callbacks {
			// Inline Variables and Methods, for solving linker problems
			inline GLFWwindow* window;
			inline graphics::Camera* camera;

			inline int* window_width;
			inline int* window_height;

			inline void framebuffer_size_callback(GLFWwindow* window, int width, int height);
			inline void mouse_callback(GLFWwindow* window, double xpos, double ypos);
			inline void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
			void setWindowSize(int* height, int* width);
			void setCallbacks(GLFWwindow* wind, graphics::Camera* cam);
			void setCallbacks(GLFWwindow* wind);
		}


} }

#endif // !WINDOW_H