/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H

#include "../../../mar.h"

namespace mar {
	struct VertexBufferElement {
		unsigned int _type;
		unsigned int _count;
		unsigned char _normalized;
	};

	class VertexBufferLayout : std::enable_shared_from_this<VertexBufferLayout> {
		std::vector<VertexBufferElement> _elements;
		unsigned int _stride;

	public:
		VertexBufferLayout() : _stride(0) {}

		const std::vector<VertexBufferElement>& getElements() const { return _elements; }
		unsigned int getStride() const { return _stride; }

		template<typename T>
		void push(unsigned int count) {
			static_assert(false);
		}

		template<>
		void push<float>(unsigned int count) {
			_elements.push_back({ GL_FLOAT, count, GL_FALSE });
			_stride += count * sizeof(GL_FLOAT);
		}

		template<>
		void push<unsigned int>(unsigned int count) {
			_elements.push_back({ GL_UNSIGNED_INT, count,  GL_FALSE });
			_stride += count * sizeof(GL_UNSIGNED_INT);
		}

		template<>
		void push<char>(unsigned int count) {
			_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
			_stride += count * sizeof(GL_UNSIGNED_BYTE);
		}
	};
}

#endif // VERTEXBUFFERLAYOUT_H