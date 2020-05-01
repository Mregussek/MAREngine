#include "VertexBuffer.h"

namespace mar {
	VertexBuffer::VertexBuffer(unsigned int sizeOfData, const float* data, size_t how_many)
		: _howMany(how_many),
		_size(sizeOfData / sizeof(float))
	{
		glGenBuffers(_howMany, &_rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
		glBufferData(GL_ARRAY_BUFFER, sizeOfData, (const void*)data, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(_howMany, &_rendererId);
	}

	void VertexBuffer::bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
	}

	void VertexBuffer::unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}