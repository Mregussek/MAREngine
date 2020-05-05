/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "VertexBuffer.h"

namespace mar {
	
	void VertexBuffer::initializeVertex(unsigned int allocationMemory) {
		if (!_initialized) {
			_allocatedMemory = allocationMemory;
			glGenBuffers(1, &_id);
			glBindBuffer(GL_ARRAY_BUFFER, _id);
			glBufferData(GL_ARRAY_BUFFER, _allocatedMemory, nullptr, GL_DYNAMIC_DRAW);
			_initialized = true;
		}
		else {
			std::cerr << "VertexBuffer is already initialized!\n";
		}
	}

	void VertexBuffer::bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, _id);
	}

	void VertexBuffer::updateDynamically(const std::vector<float>& vertices) const {
		float* vert = new float[vertices.size()];

		std::copy(vertices.begin(), vertices.end(), vert);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vert);

		delete[] vert;
	}

	void VertexBuffer::unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::close() {
		glDeleteBuffers(1, &_id);
		_initialized = false;
	}

	unsigned int VertexBuffer::getSize() const {
		int size;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		return (unsigned int)size;
	}
}