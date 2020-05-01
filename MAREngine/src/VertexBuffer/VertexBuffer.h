
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "../mar.h"

namespace mar {
	class VertexBuffer {
		unsigned int _rendererId;
		unsigned int _size;
		size_t _howMany;

	public:
		VertexBuffer(unsigned int sizeOfData, const float* data, size_t how_many = 1);
		~VertexBuffer();

		void bind() const;
		void unbind() const;

		unsigned int getSize() const {
			int size;
			glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
			return (unsigned int)size;
		}
	};
}

#endif // VERTEXBUFFER_H