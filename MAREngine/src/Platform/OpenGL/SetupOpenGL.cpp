/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#include "SetupOpenGL.h"


namespace marengine {


	void GLAPIENTRY
		MessageCallback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
	{
		constexpr bool displayDebugTypeOther{ false };
		if constexpr (!displayDebugTypeOther) {
			if (type == GL_DEBUG_TYPE_OTHER) {
				return;
			}
		}

		std::cout << "GL_CALLBACK [";
		switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			std::cout << "ERROR";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			std::cout << "DEPRECATED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			std::cout << "UNDEFINED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			std::cout << "PORTABILITY";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			std::cout << "PERFORMANCE";
			break;
		case GL_DEBUG_TYPE_OTHER:
			std::cout << "OTHER";
			break;
		}
	
		std::cout << "] id=" << id << " severity=";
		switch (severity) {
		case GL_DEBUG_SEVERITY_LOW:
			std::cout << "LOW";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			std::cout << "MEDIUM";
			break;
		case GL_DEBUG_SEVERITY_HIGH:
			std::cout << "HIGH";
			break;
		default:
			std::cout << "OTHER";
			break;
		}
		std::cout << " message: " << message << "\n-------------------------\n";
	}

	bool SetupOpenGL::init() {
		if constexpr (MAR_ENGINE_USE_GLFW_WINDOW) {
			const int32_t isGLAD_OK = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

			if (!isGLAD_OK) {
				MAR_CORE_ERROR("SETUP_OPENGL: gladLoadGLLoader with GLFW failed!");
				return false;
			}
		}
		else if constexpr (MAR_ENGINE_USE_SDL_WINDOW) {
			const int32_t isGLAD_OK = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

			if (!isGLAD_OK) {
				MAR_CORE_ERROR("SETUP_OPENGL: gladLoadGLLoader with SDL failed!");
				return false;
			}
		}
		else {
			MAR_CORE_ERROR("SETUP_OPENGL: Cannot initialize OpenGL, unsupported platform!");
			return false;
		}

		if constexpr (MAR_ENGINE_DEBUG_MODE) {
			PLATFORM_GL_FUNC( glEnable(GL_DEBUG_OUTPUT) );
			PLATFORM_GL_FUNC( glDebugMessageCallback(MessageCallback, 0) );
		}

		PLATFORM_GL_FUNC( glEnable(GL_MULTISAMPLE) );
		PLATFORM_GL_FUNC( glEnable(GL_DEPTH_TEST) ); // Enable DEPTH, in other words 3D
		PLATFORM_GL_FUNC( glEnable(GL_STENCIL_TEST) ); // Enable STENCIL, outliner
		PLATFORM_GL_FUNC( glEnable(GL_BLEND) ); // Enable loading PNG files and transparency
		PLATFORM_GL_FUNC( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

		return true;
	}

	void SetupOpenGL::clearScreen(maths::vec3 background) {
		PLATFORM_GL_FUNC( glClearColor(background.x, background.y, background.z, 1.0f) );
		PLATFORM_GL_FUNC( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) );
	}


}
