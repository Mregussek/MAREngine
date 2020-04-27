#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
	glGenBuffers(1, &_RendererId);
	glBindBuffer(GL_ARRAY_BUFFER, _RendererId);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &_RendererId);
}

void VertexBuffer::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, _RendererId);
}

void VertexBuffer::unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}