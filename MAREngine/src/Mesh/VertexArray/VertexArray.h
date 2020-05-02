/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "../../mar.h"
#include "../VertexBuffer/VertexBuffer.h"
#include "../VertexBuffer/VertexBufferLayout.h"

namespace mar {
	class VertexArray {
		unsigned int _rendererId;
		size_t _howMany;

	public:
		VertexArray(size_t how_many = 1);
		~VertexArray();

		void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

		void bind() const;
		void unbind() const;
	};
}

#endif // VERTEXARRAY_H
