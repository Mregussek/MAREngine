/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_PLATFORMS_OPENGL_DRAWING_H
#define MAR_ENGINE_PLATFORMS_OPENGL_DRAWING_H

#include "../../mar.h"
#include "../PlatformLogs.h"


namespace mar {
	namespace platforms {


		class DrawingOpenGL {
		public:

			static void drawTriangles(size_t indices_count) {
				PLATFORM_GL_FUNC( glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, nullptr) );

				PLATFORM_INFO("DRAWING_OPENGL: draw call with indices_count = {}", indices_count);
			}

			static void drawLineLoops(size_t indices_count) {
				PLATFORM_GL_FUNC( glLineWidth(20) );

				PLATFORM_GL_FUNC(glDrawElements(GL_LINE_LOOP, indices_count, GL_UNSIGNED_INT, nullptr));

				PLATFORM_GL_FUNC( glLineWidth(1) );

				PLATFORM_INFO("DRAWING_OPENGL: draw call with indices_count = {}", indices_count);
			}


		};



} }

#endif // !MAR_ENGINE_PLATFORMS_OPENGL_DRAWING_H