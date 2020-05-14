/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef VERTEXBUFFERLAYOUT_OPENGL_H
#define VERTEXBUFFERLAYOUT_OPENGL_H

#include "VertexBufferLayout.h"

namespace mar {

	//! VertexBufferLayoutOpenGL
	/*!
		VertexBufferLayoutOpenGL is used for collecting format of vertices. Main point of this class is for
		automating creating new objects.
		This is overloaded class of base one - VertexBufferLayout, which implements solution to that problem with OpenGL.
	*/
	class VertexBufferLayoutOpenGL : public VertexBufferLayout, std::enable_shared_from_this<VertexBufferLayoutOpenGL> {
		std::vector<VertexBufferElement> _elements;
		unsigned int _stride;

	public:
		//! Constructor, which initializes _stride with 0
		VertexBufferLayoutOpenGL() : _stride(0) {}

		//! Returns all elements, which help the data to be formatted properly
		/*
			\return _elements - vector of VertexBufferElement
		*/
		const std::vector<VertexBufferElement>& getElements() const override { 
			return _elements; 
		}

		//! Returns stride of data
		/*
			\return _stride - calculated stride for proper data format
		*/
		unsigned int getStride() const override { 
			return _stride; 
		}

		//! Pushes new element to layout by specifying its type and count
		/*
			\param count - value of next Vertex Attribute
			\param what - specify what value you push (float[PUSH_FLOAT], unsigned int[PUSH_UNSIGNED INT] or char[PUSH_UNSIGNED_BYTE])
		*/
		void push(unsigned int count, PushBuffer what) override {
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
				std::cerr << "Trying to push wrong element" << std::endl;
				return;
			}
		}
	};
}

#endif // !VERTEXBUFFERLAYOUT_OPENGL_H