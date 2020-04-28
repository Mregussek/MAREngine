#include "ElementBuffer.h"

ElementBuffer::ElementBuffer(const unsigned int* data, unsigned int count, size_t how_many)
: _count(count)
{
	assert(sizeof(unsigned int) == sizeof(GLuint));
	
	glGenBuffers(how_many, &_RendererId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _RendererId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
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