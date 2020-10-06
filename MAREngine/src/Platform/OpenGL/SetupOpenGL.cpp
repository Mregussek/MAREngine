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


#include "SetupOpenGL.h"


namespace mar {
	namespace platforms {


		bool SetupOpenGL::init() {
			glewExperimental = GL_TRUE;
			PLATFORM_GL_FUNC(GLenum glew_init = glewInit());

			if (glew_init != GLEW_OK)
				return false;

			PLATFORM_GL_FUNC(glEnable(GL_DEPTH_TEST)); // Enable DEPTH, in other words 3D
			PLATFORM_GL_FUNC(glEnable(GL_STENCIL_TEST)); // Enable STENCIL, outliner
			PLATFORM_GL_FUNC(glEnable(GL_BLEND)); // Enable loading PNG files and transparency
			PLATFORM_GL_FUNC(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

			return true;
		}

		void SetupOpenGL::clearScreen(maths::vec3 background) {
			PLATFORM_GL_FUNC(glClearColor(background.x, background.y, background.z, 1.0f));
			PLATFORM_GL_FUNC(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
		}


} }