/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_PLATFORMS_GLFW_WINDOW_GLFW_H
#define MAR_ENGINE_PLATFORMS_GLFW_WINDOW_GLFW_H


#include "../../mar.h"
#include "WindowCallbacks.h"
#include "../PlatformLogs.h"


namespace mar {
	namespace editor { class GUI; }

	namespace platforms {


		class WindowGLFW {
			friend class editor::GUI;

			GLFWwindow* m_window;

		public:
			bool initialize(int32_t height, int32_t width, const char* name);
			void terminate();

			bool isGoingToClose() { return glfwWindowShouldClose(m_window); }
			void close() { glfwSetWindowShouldClose(m_window, true); }

			void swapBuffers();

			void setVSync(int32_t set);

			bool isKeyPressed(int32_t key);
			bool isMousePressed(int32_t key);
		};


	}
}


#endif // !MAR_ENGINE_PLATFORMS_GLFW_WINDOW_GLFW_H
