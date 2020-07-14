/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef ELEMENTBUFFER_H
#define ELEMENTBUFFER_H

#include "../../../../mar.h"
#include "../../../../Debug/Log.h"


namespace mar {
	namespace graphics {


		/*!
		ElementBuffer

			ElementBuffer is used to store indices in GPU memory and dynamically set new values if needed.
			Other common name for ElementBuffer is IndexBuffer.
			This is just base class for other implementations of ElementBuffer.
		*/
		class ElementBuffer : std::enable_shared_from_this<ElementBuffer> {
		public:
			//! Default constructor. For initialization use initializeElement() method.
			ElementBuffer() = default;

			/*
			This method should initialize ElementBufferObject to some id instance, define data on GPU, reserve memory on GPU,
			also tell the GPU that user will change resources dynamically. Method must be overloaded!

				\param allocationMemory - memory (in bytes!), which will be allocated in GPU
			*/
			virtual void initializeElement(const unsigned int allocationMemory) { }

			/*
			Method must redefine some or all of data stored in EBO. Method must be overloaded!

				\param data - indices, which will be drawn
			*/
			virtual void updateDynamically(const std::vector<unsigned int>& data) const { }

			//! Method should reset buffer. Must be overloaded!
			virtual void resetBuffer() { }

			//! Method must bind class m_id member to target, which is EBO. It must be overloaded!
			virtual void bind() const { }

			//! Method must unbind currently used EBO and restore memory usage for that buffer. Should be overloaded!
			virtual void unbind() const { }

			//! This method should delete everything from GPU, that is bounded to EBO
			virtual void close() { }
		};


} }

#endif // !ELEMENTBUFFER_H
