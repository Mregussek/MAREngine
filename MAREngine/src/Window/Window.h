/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_WINDOW_WINDOW_H
#define MAR_ENGINE_WINDOW_WINDOW_H


#include "../mar.h"
#include "WindowLogs.h"
#include "../Platform/GLFW/WindowGLFW.h"
#include "../Platform/OpenGL/SetupOpenGL.h"


namespace mar {
	namespace editor { class GUI; }

	namespace window {
	
		class Input;


		class Window {
			friend class Input;
			friend class editor::GUI;

			static Window s_instance;

			platforms::WindowGLFW m_window;
			uint32_t m_width;
			uint32_t m_height;
			maths::vec3 m_background;

		public:
			static Window& getInstance() { return s_instance; }
			
			void updateBackgroundColor(maths::vec3 new_back) { m_background = new_back; }

			void initialize(int32_t width, int32_t height, const char* name) {
				m_width = width;
				m_height = height;

				bool is_glfw_fine = m_window.initialize(width, height, name);
				if (!is_glfw_fine) {
					WINDOW_ERROR("WINDOW: Cannot initialize GLFW window!");
					char c = getchar();
					exit(0);
				}

				WINDOW_INFO("WINDOW: initialized window!");

				bool is_opengl_fine = platforms::SetupOpenGL::init();
				if (!is_opengl_fine) {
					WINDOW_ERROR("WINDOW: Cannot initialize OpenGL!");
					char c = getchar();
					exit(0);
				}

				WINDOW_INFO("WINDOW: initialized OpenGL!");
			}

			void shutdown() {
				m_window.terminate();

				WINDOW_INFO("WINDOW: closed Window!");
			}

			void clear() {
				platforms::SetupOpenGL::clearScreen(m_background);

				WINDOW_TRACE("WINDOW: is clearing screen");
			}

			void update() {
				m_window.swapBuffers();

				WINDOW_TRACE("WINDOW: updating buffers and callbacks");
			}

			bool isGoingToClose() {
				return m_window.isGoingToClose();
			}

			void endRenderLoop() {
				m_window.close();

				WINDOW_INFO("WINDOW: ending render loop");
			}
		};


} }


#endif // !MAR_ENGINE_WINDOW_WINDOW_H