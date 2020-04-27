#include "VertexArray.h"

VertexArray::VertexArray() {
	glGenVertexArrays(1, &_rendererId);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &_rendererId);
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	bind();
	vb.bind();
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& elem = elements[i];
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(i, elem._count, elem._type, elem._normalized, 
			layout.getStride(), (const void*) offset);
		offset += elem._count * VertexBufferElement::getSizeOfType(elem._type);
	}

}

void VertexArray::bind() const {
	glBindVertexArray(_rendererId);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}
