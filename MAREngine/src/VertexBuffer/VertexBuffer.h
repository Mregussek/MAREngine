
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "../mre.h"

class VertexBuffer {
	unsigned int _rendererId;

public:
	VertexBuffer(unsigned int size, const void* data);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
};

#endif // VERTEXBUFFER_H