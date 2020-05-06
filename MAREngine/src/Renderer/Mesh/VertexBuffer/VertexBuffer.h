/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "../../../mar.h"

namespace mar {
	class VertexBuffer : std::enable_shared_from_this<VertexBuffer> {
	public:
		virtual void initializeVertex(unsigned int allocationMemory) { }

		virtual void bind() const { }
		virtual void updateDynamically(const std::vector<float>& vertices) const { }
		virtual void unbind() const { }
		virtual void close() { }

		virtual unsigned int getSize() const { return 0; }
	};
}

#endif // VERTEXBUFFER_H