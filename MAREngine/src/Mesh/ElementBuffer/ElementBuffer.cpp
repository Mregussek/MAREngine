/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "ElementBuffer.h"

namespace mar {
	ElementBuffer::ElementBuffer(unsigned int sizeOfData, const unsigned int* data, size_t how_many)
		: _numberOfIndices(sizeOfData / sizeof(decltype(sizeOfData))),
		_allocatedMemory(sizeOfData)
	{
		assert(sizeof(unsigned int) == sizeof(GLuint));

		glGenBuffers(how_many, &_RendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _RendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfData, data, GL_STATIC_DRAW);
	}

	ElementBuffer::ElementBuffer(const std::vector<unsigned int>& data, const unsigned int allocationMemory)
		: _numberOfIndices(data.size()),
		_allocatedMemory(allocationMemory)
	{
		unsigned int* indices = new unsigned int[data.size()];
		std::copy(data.begin(), data.end(), indices);

		glGenBuffers(1, &_RendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _RendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, allocationMemory, indices, GL_STATIC_DRAW);
		delete[] indices;
	}

	void ElementBuffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _RendererId);
	}

	void ElementBuffer::unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void ElementBuffer::close() const {
		glDeleteBuffers(1, &_RendererId);
	}
}
