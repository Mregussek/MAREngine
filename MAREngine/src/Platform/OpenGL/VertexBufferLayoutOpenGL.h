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


#ifndef MAR_ENGINE_PLATFORMS_VERTEXBUFFERLAYOUT_OPENGL_H
#define MAR_ENGINE_PLATFORMS_VERTEXBUFFERLAYOUT_OPENGL_H

#include "../../mar.h"
#include "../PlatformLogs.h"


namespace mar {
	namespace platforms {


		/*!
		VertexBufferElement

			VertexBufferElement stores data, which will be put in GPU. Helps set up
			VertexAttribPointer.
		*/
		struct VertexBufferElement {
			uint32_t _type;
			uint32_t _count;
			unsigned char _normalized;
		};

		/*!
		PushBuffer

			PushBuffer helps in deciding what type of value
			we are pushing to buffer.
		*/
		enum class PushBuffer {
			PUSH_FLOAT,
			PUSH_UNSIGNED_INT,
			PUSH_UNSIGNED_BYTE
		};

		/*!
		VertexBufferLayoutOpenGL

			VertexBufferLayoutOpenGL is used for collecting format of vertices. Main point of this class is for
			automating creating new objects.
			This is overloaded class of base one - VertexBufferLayout, which implements solution to that problem with OpenGL.
		*/
		class VertexBufferLayoutOpenGL {
			std::vector<VertexBufferElement> _elements;
			uint32_t _stride{ 0 };

		public:
			//! Constructor, which initializes _stride with 0
			VertexBufferLayoutOpenGL() = default;

			/*
			Returns all elements, which help the data to be formatted properly

				\return _elements - vector of VertexBufferElement
			*/
			const std::vector<VertexBufferElement>& getElements() const {
				return _elements;
			}

			/*
			Returns stride of data

				\return _stride - calculated stride for proper data format
			*/
			uint32_t getStride() const {
				return _stride;
			}

			/*
			Pushes new element to layout by specifying its type and count

				\param count - value of next Vertex Attribute
				\param what - specify what value you pushData (float[PUSH_FLOAT], uint32_t[PUSH_uint32_t] or char[PUSH_UNSIGNED_BYTE])
			*/
			void push(uint32_t count, PushBuffer what) {
				if (what == PushBuffer::PUSH_FLOAT) {
					_elements.push_back({ GL_FLOAT, count, GL_FALSE });
					_stride += count * sizeof(GL_FLOAT);
				}
				else if (what == PushBuffer::PUSH_UNSIGNED_INT) {
					_elements.push_back({ GL_UNSIGNED_INT, count,  GL_FALSE });
					_stride += count * sizeof(GL_UNSIGNED_INT);
				}
				else if (what == PushBuffer::PUSH_UNSIGNED_BYTE) {
					_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
					_stride += count * sizeof(GL_UNSIGNED_BYTE);
				}
				else {
					PLATFORM_ERROR("VertexBufferLayout_OpenGL: Trying to push wrong element");
					return;
				}
			}
		};


} }

#define PUSH_BUFFER_FLOAT ::mar::platforms::PushBuffer::PUSH_FLOAT
#define PUSH_BUFFER_UNSIGNED_INT ::mar::platforms::PushBuffer::PUSH_UNSIGNED_INT
#define PUSH_BUFFER_UNSIGNED_BYTE ::mar::platforms::PushBuffer::PUSH_UNSIGNED_BYTE

#endif // !MAR_ENGINE_VERTEXBUFFERLAYOUT_OPENGL_H