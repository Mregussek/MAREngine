/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef ELEMENTBUFFER_H
#define ELEMENTBUFFER_H

#include "../../../mar.h"

namespace mar {
	class ElementBuffer : std::enable_shared_from_this<ElementBuffer> {
		unsigned int _id;
		unsigned int _allocatedMemory;
		bool _initialized = false;

	public:
		ElementBuffer() = default;

		void initializeElement(const std::vector<unsigned int>& data, 
			const unsigned int allocationMemory);

		void bind() const;
		void unbind() const;
		void close();
	};
}


#endif // ELEMENTBUFFER_H