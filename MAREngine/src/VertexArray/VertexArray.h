
#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "../mre.h"
#include "../VertexBuffer/VertexBuffer.h"
#include "../VertexBuffer/VertexBufferLayout.h"

class VertexArray {
	unsigned int _rendererId;
public:
	VertexArray(size_t how_many = 1);
	~VertexArray();

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void bind() const;
	void unbind() const;
};

#endif // VERTEXARRAY_H