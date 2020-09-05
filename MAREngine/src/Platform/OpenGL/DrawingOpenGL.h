/**
 *           MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


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