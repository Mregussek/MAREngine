/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "../../../mar.h"

namespace mar {
	class VertexArray : std::enable_shared_from_this<VertexArray> {
	public:
		VertexArray() = default;
		virtual ~VertexArray() { }

		virtual void initializeArrayBuffer() { }
		virtual void addBuffer(const std::shared_ptr<VertexBufferLayout>& layout) { }

		virtual void bind() const { }
		virtual void unbind() const { }
	};
}


#endif // !VERTEXARRAY_H
