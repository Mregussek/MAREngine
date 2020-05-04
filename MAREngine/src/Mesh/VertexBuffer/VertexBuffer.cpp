/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "VertexBuffer.h"

namespace mar {
	VertexBuffer::VertexBuffer(unsigned int sizeOfData, const float* data, size_t how_many)
		: _howMany(how_many),
		_size(sizeOfData / sizeof(float)),
		_allocatedMemory(sizeOfData)
	{
		glGenBuffers(_howMany, &_rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
		glBufferData(GL_ARRAY_BUFFER, sizeOfData, (const void*)data, GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(const std::vector<float>& data, size_t how_many)
		: _howMany(how_many),
		_size(data.size()),
		_allocatedMemory(_size * sizeof(float))
	{
		float* vertices = new float[data.size()];
		std::copy(data.begin(), data.end(), vertices);

		glGenBuffers(_howMany, &_rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
		glBufferData(GL_ARRAY_BUFFER, _size * sizeof(float), (const void*)vertices, GL_STATIC_DRAW);
		delete[] vertices;
	}

	VertexBuffer::VertexBuffer(unsigned int allocationMemory)
		: _howMany(1),
		_size(0),
		_allocatedMemory(allocationMemory)
	{
		glGenBuffers(1, &_rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, _rendererId); // dynamic draw for batch rendering
		glBufferData(GL_ARRAY_BUFFER, _allocatedMemory, nullptr, GL_DYNAMIC_DRAW); // allocate memory on gpu
	}

	void VertexBuffer::bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
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

	void VertexBuffer::close() const {
		glDeleteBuffers(_howMany, &_rendererId);
	}
}