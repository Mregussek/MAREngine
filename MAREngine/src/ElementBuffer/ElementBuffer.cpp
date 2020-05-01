#include "ElementBuffer.h"

namespace mar {
	ElementBuffer::ElementBuffer(unsigned int sizeOfData, const unsigned int* data, size_t how_many)
		: _numberOfIndices(sizeOfData / sizeof(decltype(sizeOfData)))
	{
		assert(sizeof(unsigned int) == sizeof(GLuint));

		glGenBuffers(how_many, &_RendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _RendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfData, data, GL_STATIC_DRAW);
	}

	ElementBuffer::~ElementBuffer() {
		glDeleteBuffers(1, &_RendererId);
	}

	void ElementBuffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _RendererId);
	}

	void ElementBuffer::unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
