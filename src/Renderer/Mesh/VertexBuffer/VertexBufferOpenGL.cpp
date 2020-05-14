/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "VertexBufferOpenGL.h"

namespace mar {
	
	void VertexBufferOpenGL::initializeVertex(unsigned int allocationMemory) {
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

	void VertexBufferOpenGL::bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, _id);
	}

	void VertexBufferOpenGL::updateDynamically(const std::vector<float>& vertices) const {
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
	}

	void VertexBufferOpenGL::unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBufferOpenGL::close() {
		glDeleteBuffers(1, &_id);
		_initialized = false;
	}

	unsigned int VertexBufferOpenGL::getSize() const {
		int size;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		return (unsigned int)size;
	}
}