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


#ifndef MAR_ENGINE_PLATFORMS_VERTEXBUFFER_OPENGL_H
#define MAR_ENGINE_PLATFORMS_VERTEXBUFFER_OPENGL_H

#include "../../mar.h"
#include "../PlatformLogs.h"

namespace mar {
	namespace platforms {


		/*!
		VertexBufferOpenGL

			VertexBufferOpenGL is used to store vertices on the GPU memory and as being the source for vertex array data.
			This is overloaded class of base one - VertexBuffer, which implements solution to that problem with OpenGL.
		*/
		class VertexBufferOpenGL {
			uint32_t _id;
			uint32_t _size;
			uint32_t _allocatedMemory;
			bool _initialized = false;

		public:
			//! Default constructor. For initialization use initialize() method.
			VertexBufferOpenGL() = default;

			/*
			Initializes VertexBufferObject to m_id and reserves memory on GPU, also tells
			the GPU that user will change resources dynamically.

				\param allocationMemory - size of memory (bytes!), which will be allocated on GPU
			*/
			void initialize(uint32_t allocationMemory);

			//! Binds class m_id member to target, which is VBO
			void bind() const;

			/*
			Redefines some or all of data stored in VBO.

				\param vertices - object, which will replace data stored in GPU
			*/
			void update(const std::vector<float>& vertices) const;

			//! Method resets buffer.
			void resetBuffer();

			//! Unbinds currently used VBO and restore memory usage for that buffer
			void unbind() const;

			//! Deletes VBO
			void close();

			/*
			Get size (in bytes!), which is allocated on GPU

				\return size - returns size allocated on GPU
			*/
			int32_t getSize() const;
		};


} }

#endif // MAR_ENGINE_VERTEXBUFFER_OPENGL_H