/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "../../mar.h"

namespace mar {
	class VertexBuffer {
		unsigned int _rendererId;
		unsigned int _size;
		size_t _howMany;

	public:
		VertexBuffer() = default;
		VertexBuffer(unsigned int sizeOfData, const float* data, size_t how_many = 1);
		VertexBuffer(const std::vector<float>& data, size_t how_many = 1);

		void bind() const;
		void unbind() const;
		void close() const;

		unsigned int getSize() const {
			int size;
			glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
			return (unsigned int)size;
		}
	};
}

#endif // VERTEXBUFFER_H