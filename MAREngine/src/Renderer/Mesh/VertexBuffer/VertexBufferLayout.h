/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H

#include "../../../mar.h"

namespace mar {
	struct VertexBufferElement {
		unsigned int _type;
		unsigned int _count;
		unsigned char _normalized;
	};

	enum class PushBuffer {
		PUSH_FLOAT,
		PUSH_UNSIGNED_INT,
		PUSH_UNSIGNED_BYTE
	};

	class VertexBufferLayout {
	public:
		VertexBufferLayout() = default;

		virtual const std::vector<VertexBufferElement>& getElements() const { return std::vector<VertexBufferElement>(); }
		virtual unsigned int getStride() const { return 0; }

		virtual void push(unsigned int count, PushBuffer what) { }
	};
}

#endif // VERTEXBUFFERLAYOUT_H