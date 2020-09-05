/**
 *				MAREngine - open source 3D game engine
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


#ifndef MAR_ENGINE_PLATFORMS_VERTEXARRAY_OPENGL_H
#define MAR_ENGINE_PLATFORMS_VERTEXARRAY_OPENGL_H

#include "../../mar.h"
#include "VertexBufferLayoutOpenGL.h"
#include "../PlatformLogs.h"


namespace mar {
	namespace platforms {


		/*!
		VertexArrayOpenGL

			VertexArrayOpenGL is a object needed for storing state for supplying vertex data. Should contain
			format of vertex data, by providing vertex arrays.
			This is overloaded class of base one - VertexArrayOpenGL, which implements solution to that problem with OpenGL.
		*/
		class VertexArrayOpenGL {
			uint32_t _rendererId;

		public:
			//! Default constructor. For initialization use initialize() method.
			VertexArrayOpenGL() = default;

			//! Generates and binds VAO
			void initialize();

			//! Deletes VAO
			void close();

			/*
			Defines array of generic vertex attribute data and enables it

				\param layout - specifies, how data should be formatted
			*/
			void addBuffer(const VertexBufferLayoutOpenGL& layout);

			//! Binds class m_id member to target, which is VAO
			void bind() const;

			//! Unbind currently used VAO, break the existing VAO binding
			void unbind() const;
		};


} }

#endif // !MAR_ENGINE_VERTEXARRAY_OPENGL_H
