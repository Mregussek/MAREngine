/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef ELEMENTBUFFER_H
#define ELEMENTBUFFER_H

#include "../../mar.h"

namespace mar {
	class ElementBuffer {
		unsigned int _RendererId;
		unsigned int _numberOfIndices;

	public:
		ElementBuffer() = default;
		ElementBuffer(unsigned int sizeOfData, const unsigned int* data, size_t how_many = 1);
		ElementBuffer(const std::vector<unsigned int>& data, size_t how_many = 1);

		void bind() const;
		void unbind() const;
		void close() const;

		const unsigned int getIndicesNumber() const { return _numberOfIndices; }
	};
}


#endif // ELEMENTBUFFER_H