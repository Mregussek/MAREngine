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


#ifndef MAR_ENGINE_PLATFORMS_ELEMENTBUFFER_OPENGL_H
#define MAR_ENGINE_PLATFORMS_ELEMENTBUFFER_OPENGL_H


#include "../../mar.h"
#include "../PlatformLogs.h"


namespace mar {
	namespace platforms {


		/*!
			ElementBufferOpenGL

			ElementBufferOpenGL is used to store indices in GPU memory and dynamically set new values if needed.
			Other common name for ElementBuffer is IndexBuffer.
			This is overloaded class of base one - ElementBuffer, which implements solution to that problem with OpenGL.
		*/
		class ElementBufferOpenGL {
			uint32_t _id;
			uint32_t _allocatedMemory;
			bool _initialized = false;

		public:
			//! Default constructor. For initialization use initialize() method.
			ElementBufferOpenGL() = default;

			/*
			Initializes ElementBufferObject to m_id, defines data on GPU, reserves memory on GPU,
			also tells the GPU that user will change resources dynamically.

				\param allocationMemory - memory (in bytes!), which will be allocated in GPU
			*/
			void initialize(const uint32_t allocationMemory);

			/*
			Redefines some or all of data stored in EBO.

				\param data - indices, which will be drawn
			*/
			void update(const std::vector<uint32_t>& data) const;

			//! Method resets buffer.
			void resetBuffer();

			//! Binds class m_id member to target, which is EBO
			void bind() const;

			//! Unbind currently used EBO and restore memory usage for that buffer
			void unbind() const;

			//! Deletes EBO
			void close();
		};


} }


#endif // !ELEMENTBUFFER_OPENGL_H