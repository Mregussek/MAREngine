
#ifndef ELEMENTBUFFER_H
#define ELEMENTBUFFER_H

#include "../mre.h"

class ElementBuffer {
	unsigned int _RendererId;
	unsigned int _count;

public:
	ElementBuffer(const unsigned int* data, unsigned int count, size_t how_many = 1);
	~ElementBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return _count;  }
};

#endif // ELEMENTBUFFER_H