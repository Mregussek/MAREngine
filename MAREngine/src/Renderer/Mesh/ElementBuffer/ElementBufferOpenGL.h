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
		//! Default constructor. For initialization use initializeElement() method.
		ElementBufferOpenGL() = default;

		//! Initializes ElementBufferObject to _id, define data on GPU, reserves memory on GPU, 
		//! also tells the GPU that user will change resources dynamically. 
		/*
			\param allocationMemory - memory (in bytes!), which will be allocated in GPU
		*/
		void initializeElement(const unsigned int allocationMemory) override;

		//! Redefines some or all of data stored in EBO.
		/*
			\param data - indices, which will be drawn
		*/
		void updateDynamically(const std::vector<unsigned int>& data) const override;

		//! Binds class _id member to target, which is EBO
		void bind() const override;

		//! Unbind currently used EBO and restore memory usage for that buffer
		void unbind() const override;

		//! Deletes EBO
		void close() override;
	};
}


#endif // !ELEMENTBUFFER_OPENGL_H