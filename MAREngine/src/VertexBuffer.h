
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "mre.h"

class VertexBuffer {
	unsigned int _RendererId;

public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
};

#endif // VERTEXBUFFER_H