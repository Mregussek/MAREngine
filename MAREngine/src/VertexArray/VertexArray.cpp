#include "VertexArray.h"

VertexArray::VertexArray(size_t how_many)
	: _howMany(how_many)
{
	glGenVertexArrays(how_many, &_rendererId);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(_howMany, &_rendererId);
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	
	bind();
	vb.bind();

	for (unsigned int i = 0; i < elements.size(); i++) {
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(i, 
			elements[i]._count, elements[i]._type, elements[i]._normalized,
			layout.getStride(), (const void*) offset);
		offset += elements[i]._count * VertexBufferElement::getSizeOfType(elements[i]._type);
	}
}

void VertexArray::bind() const {
	glBindVertexArray(_rendererId);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}
