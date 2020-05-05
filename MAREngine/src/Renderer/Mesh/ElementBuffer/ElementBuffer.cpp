/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "ElementBuffer.h"

namespace mar {
	
	void ElementBuffer::initializeElement(const std::vector<unsigned int>& data, const unsigned int allocationMemory) {
		if (!_initialized) {
			_allocatedMemory = allocationMemory;

			unsigned int* indices = new unsigned int[data.size()];
			std::copy(data.begin(), data.end(), indices);

			glGenBuffers(1, &_id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, _allocatedMemory, indices, GL_STATIC_DRAW);

			delete[] indices;
			_initialized = true;
		}
		else {
			std::cerr << "ElementBuffer is already initialized!\n";
		}
	}

	void ElementBuffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	}

	void ElementBuffer::unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void ElementBuffer::close() {
		glDeleteBuffers(1, &_id);
		_initialized = false;
	}
}