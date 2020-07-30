/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_VERTEXBUFFERLAYOUT_OPENGL_H
#define MAR_ENGINE_VERTEXBUFFERLAYOUT_OPENGL_H

#include "VertexBufferLayout.h"


namespace mar {
	namespace graphics {


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
					GRAPHICS_ERROR("VertexBufferLayout_OpenGL: Trying to push wrong element");
					return;
				}
			}
		};


} }

#endif // !MAR_ENGINE_VERTEXBUFFERLAYOUT_OPENGL_H