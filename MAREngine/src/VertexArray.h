
#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "mre.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {
	unsigned int _rendererId;
public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void bind() const;
	void unbind() const;
};

#endif // VERTEXARRAY_H
