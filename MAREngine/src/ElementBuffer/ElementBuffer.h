
#ifndef ELEMENTBUFFER_H
#define ELEMENTBUFFER_H

#include "../mar.h"

namespace mar {
	class ElementBuffer {
		unsigned int _RendererId;
		unsigned int _numberOfIndices;

	public:
		ElementBuffer(unsigned int sizeOfData, const unsigned int* data, size_t how_many = 1);
		~ElementBuffer();

		void bind() const;
		void unbind() const;

		const unsigned int getIndicesNumber() const { return _numberOfIndices; }
	};
}


#endif // ELEMENTBUFFER_H