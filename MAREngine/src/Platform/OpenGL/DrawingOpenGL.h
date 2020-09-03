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
				PLATFORM_GL_FUNC( glStencilFunc(GL_ALWAYS, 1, 0xFF) );
				PLATFORM_GL_FUNC( glStencilMask(0xFF) );

				PLATFORM_GL_FUNC( glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, nullptr) );

				PLATFORM_INFO("DRAWING_OPENGL: draw call with indices_count = {}", indices_count);
			}

			static void drawLineLoops(size_t indices_count, float line_width) {
				PLATFORM_GL_FUNC( glLineWidth(line_width) );

				PLATFORM_GL_FUNC( glDrawElements(GL_LINE_LOOP, indices_count, GL_UNSIGNED_INT, nullptr) );

				PLATFORM_GL_FUNC( glLineWidth(1.0f) );

				PLATFORM_INFO("DRAWING_OPENGL: draw call with indices_count = {}", indices_count);
			}

			static void drawOutline(size_t indices_count) {
				PLATFORM_GL_FUNC( glStencilFunc(GL_NOTEQUAL, 1, 0xFF) );
				PLATFORM_GL_FUNC( glStencilMask(0x00) );
				PLATFORM_GL_FUNC( glDisable(GL_DEPTH_TEST) );
	
				drawLineLoops(indices_count, 0.25f);

				PLATFORM_GL_FUNC( glStencilMask(0xFF) );
				PLATFORM_GL_FUNC( glStencilFunc(GL_ALWAYS, 1, 0xFF) );
				PLATFORM_GL_FUNC( glEnable(GL_DEPTH_TEST) );
				PLATFORM_GL_FUNC( glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE) );

				PLATFORM_INFO("DRAWING_OPENGL: draw call with indices_count = {}", indices_count);
			}


		};



} }

#endif // !MAR_ENGINE_PLATFORMS_OPENGL_DRAWING_H