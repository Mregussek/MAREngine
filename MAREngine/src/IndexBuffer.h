
#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include "mre.h"

class IndexBuffer {
	unsigned int _RendererId;
	unsigned int _count;

public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return _count;  }
};

#endif // INDEXBUFFER_H