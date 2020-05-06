/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef VERTEXBUFFERLAYOUT_OPENGL_H
#define VERTEXBUFFERLAYOUT_OPENGL_H

#include "VertexBufferLayout.h"

namespace mar {

	class VertexBufferLayoutOpenGL : public VertexBufferLayout, std::enable_shared_from_this<VertexBufferLayoutOpenGL> {
		std::vector<VertexBufferElement> _elements;
		unsigned int _stride;

	public:
		VertexBufferLayoutOpenGL() : _stride(0) {}

		const std::vector<VertexBufferElement>& getElements() const override { 
			return _elements; 
		}

		unsigned int getStride() const override { 
			return _stride; 
		}

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