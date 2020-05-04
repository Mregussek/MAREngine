/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "VertexArray.h"

namespace mar {
	VertexArray::VertexArray(size_t how_many)
		: _howMany(how_many)
	{
		glGenVertexArrays(how_many, &_rendererId);
		glBindVertexArray(_rendererId);
	}

	VertexArray::~VertexArray() {
		glDeleteVertexArrays(_howMany, &_rendererId);
	}

	void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
		const auto& elements = layout.getElements();
		unsigned int offset = 0;

		for (unsigned int i = 0; i < elements.size(); i++) {
			glVertexAttribPointer(i, elements[i]._count, 
				elements[i]._type, elements[i]._normalized,
				layout.getStride(), (const void*)offset);

			glEnableVertexAttribArray(i);
			offset += elements[i]._count * sizeof(elements[i]._type); //VertexBufferElement::getSizeOfType(elements[i]._type);
		}
	}

	void VertexArray::addBufferBatch(const VertexBuffer& vb, const VertexBufferLayout& layout) {
		const auto& elements = layout.getElements();
		unsigned int offset = 0;

		for (unsigned int i = 0; i < elements.size(); i++) {
			glVertexAttribPointer(i, elements[i]._count,
				elements[i]._type, elements[i]._normalized,
				layout.getStride(), (const void*)offset);

			glEnableVertexAttribArray(i);
			offset += elements[i]._count * sizeof(elements[i]._type); //VertexBufferElement::getSizeOfType(elements[i]._type);
		}
	}

	void VertexArray::bind() const {
		glBindVertexArray(_rendererId);
	}

	void VertexArray::unbind() const {
		glBindVertexArray(0);
	}
}
