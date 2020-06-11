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
			GLFWwindow* _window;
			int _width;
			int _height;
			const char* _windowName;

		public:
			Window() = default;

			void initialize(const int& H, const int& W, const char* wN, graphics::Camera* camera = nullptr);
			void shutdown();

			void swapBuffers();
			void clearScreen();

			GLFWwindow* getWindow() const;
			const bool shouldClose() const;
		};

		namespace callbacks {
			// Inline Variables and Methods, for solving linker problems
			inline GLFWwindow* window;
			inline graphics::Camera* camera;
			inline void framebuffer_size_callback(GLFWwindow* window, int width, int height);
			inline void mouse_callback(GLFWwindow* window, double xpos, double ypos);
			inline void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
			void setCallbacks(GLFWwindow* wind, graphics::Camera* cam);
			void setCallbacks(GLFWwindow* wind);
		}


} }

#endif // !WINDOW_H