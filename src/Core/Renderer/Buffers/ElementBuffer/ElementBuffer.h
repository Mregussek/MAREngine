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

			Example usage:

			ElementBuffer<ElementBufferOpenGL> m_ebo;

			This is how we can call OpenGL implementation of ElementBuffer.
		*/
		template<typename EBO>
		class ElementBuffer {
			EBO m_ebo;

		public:
			//! Default constructor. For initialization use initialize() method.
			ElementBuffer() = default;

			/*
			This method should initialize ElementBufferObject to some id instance, reserve memory on GPU,
			also tell the GPU that user will change resources dynamically. Must be overloaded!

				\param allocationMemory - memory (in bytes!), which will be allocated in GPU
			*/
			void initialize(const uint32_t allocationMemory) { 
				m_ebo.initialize(allocationMemory); 
			}

			/*
			Method must redefine some or all of data stored in EBO. Must be overloaded!

				\param data - indices, which will be drawn
			*/
			void update(const std::vector<uint32_t>& data) const {
				m_ebo.update(data);
			}

			//! Method should reset buffer. Must be overloaded!
			void resetBuffer() { 
				m_ebo.resetBuffer();
			}

			//! Method must bind class m_id member to target, which is EBO. Must be overloaded!
			void bind() const { 
				m_ebo.bind();
			}

			//! Method must unbind currently used EBO and restore memory usage for that buffer. Must be overloaded!
			void unbind() const {
				m_ebo.unbind();
			}

			//! This method should delete everything from GPU, that is bounded to EBO. Must be overloaded!
			void close() {
				m_ebo.close();
			}
		};


} }

#endif // !ELEMENTBUFFER_H
