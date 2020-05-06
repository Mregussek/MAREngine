/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef ELEMENTBUFFER_H
#define ELEMENTBUFFER_H

#include "../../../mar.h"

namespace mar {
	class ElementBuffer : std::enable_shared_from_this<ElementBuffer> {
	public:
		ElementBuffer() = default;

		virtual void initializeElement(const std::vector<unsigned int>& data,
			const unsigned int allocationMemory) { }

		virtual void bind() const { }
		virtual void unbind() const { }
		virtual void close() { }
	};
}

#endif // !ELEMENTBUFFER_H
