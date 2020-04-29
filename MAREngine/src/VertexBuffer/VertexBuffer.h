
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "../mar.h"

class VertexBuffer {
	unsigned int _rendererId;
	size_t _howMany;

public:
	VertexBuffer(unsigned int size, const void* data, size_t how_many = 1);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
};

#endif // VERTEXBUFFER_H