/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_PLATFORMS_OPENGL_INITIALIZATION_H
#define MAR_ENGINE_PLATFORMS_OPENGL_INITIALIZATION_H


#include "../../mar.h"
#include "../PlatformLogs.h"


namespace mar {
	namespace platforms {


		class SetupOpenGL {
		public:
			static bool init() {
				glewExperimental = GL_TRUE;
				PLATFORM_GL_FUNC( GLenum glew_init = glewInit() );

				if (glew_init != GLEW_OK)
					return false;

				PLATFORM_GL_FUNC( glEnable(GL_DEPTH_TEST) ); // Enable DEPTH, in other words 3D
				PLATFORM_GL_FUNC( glEnable(GL_STENCIL_TEST) ); // Enable STENCIL, outliner
				PLATFORM_GL_FUNC( glEnable(GL_BLEND) ); // Enable loading PNG files and transparency
				PLATFORM_GL_FUNC( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

				return true;
			}

			static void clearScreen(const maths::vec3& background) {
				PLATFORM_GL_FUNC( glClearColor(background.x, background.y, background.z, 1.0f) );
				PLATFORM_GL_FUNC( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) );
			}
		};


} }


#endif // !MAR_ENGINE_PLATFORMS_OPENGL_INITIALIZATION_H
