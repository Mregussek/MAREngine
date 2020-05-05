/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "../../../mar.h"

namespace mar {
	class VertexBuffer : std::enable_shared_from_this<VertexBuffer> {
		unsigned int _id;
		unsigned int _size;
		unsigned int _allocatedMemory;
		bool _initialized = false;

	public:
		VertexBuffer() = default;

		void initializeVertex(unsigned int allocationMemory);

		void bind() const;
		void updateDynamically(const std::vector<float>& vertices) const;
		void unbind() const;
		void close();

		unsigned int getSize() const;
	};
}

#endif // VERTEXBUFFER_H