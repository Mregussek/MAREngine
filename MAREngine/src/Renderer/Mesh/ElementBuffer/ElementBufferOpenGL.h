/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef ELEMENTBUFFER_OPENGL_H
#define ELEMENTBUFFER_OPENGL_H

#include "ElementBuffer.h"

namespace mar {
	class ElementBufferOpenGL : public ElementBuffer, std::enable_shared_from_this<ElementBufferOpenGL> {
		unsigned int _id;
		unsigned int _allocatedMemory;
		bool _initialized = false;

	public:
		ElementBufferOpenGL() = default;

		void initializeElement(const std::vector<unsigned int>& data, 
			const unsigned int allocationMemory) override;

		void bind() const override;
		void unbind() const override;
		void close() override;
	};
}


#endif // !ELEMENTBUFFER_OPENGL_H