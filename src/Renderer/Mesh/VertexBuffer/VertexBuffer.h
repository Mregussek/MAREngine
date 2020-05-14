/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "../../../mar.h"

namespace mar {

	//! VertexBuffer
	/*!
		VertexBuffer is used to store vertices on the GPU memory and as being the source for vertex array data.
		This is just base class for other implementations of VertexBuffer.
	*/
	class VertexBuffer : std::enable_shared_from_this<VertexBuffer> {
	public:
		//! Method should initialize VertexBufferObject to unique id and reserve memory on GPU, also tell
		//! the GPU that user will change resources dynamically. Method must be overloaded!
		/*
			\param allocationMemory - size of memory (bytes!), which will be allocated on GPU
		*/
		virtual void initializeVertex(unsigned int allocationMemory) { }

		//! Method must bind class id member to target, which is VBO. Method must be overloaded!
		virtual void bind() const { }

		//! Method must redefine some or all of data stored in VBO. Method must be overloaded!
		/*
			\param vertices - object, which will replace data stored in GPU
		*/
		virtual void updateDynamically(const std::vector<float>& vertices) const { }

		//! Method should unbind currently used VBO and restore memory usage for that buffer. 
		//! Method must be overloaded!
		virtual void unbind() const { }

		//! Method should delete VBO. Method must be overloaded!
		virtual void close() { }

		//! Method should return size (in bytes!), which is allocated on GPU. Method must be overloaded!
		/*
			\return size - returns size allocated on GPU
		*/
		virtual unsigned int getSize() const { return 0; }
	};
}

#endif // VERTEXBUFFER_H