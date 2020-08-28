/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_VERTEXBUFFER_H
#define MAR_ENGINE_VERTEXBUFFER_H

#include "../../../../../mar.h"
#include "../../../GraphicsLogs.h"


namespace mar {
	namespace graphics {


		/*!
		VertexBuffer

			VertexBuffer is used to store vertices on the GPU memory and as being the source for vertex array data.
			This is just base class for other implementations of VertexBuffer.

			Example Usage:

			VertexBuffer<VertexBufferOpenGL> m_vbo;
		*/
		template<typename VBO>
		class VertexBuffer {
			VBO m_vbo;
			
		public:

			/*
			Method should initialize VertexBufferObject to unique id and reserve memory on GPU, also tell
			the GPU that user will change resources dynamically. Method must be overloaded!

				\param allocationMemory - size of memory (bytes!), which will be allocated on GPU
			*/
			void initialize(uint32_t allocationMemory) { 
				m_vbo.initialize(allocationMemory);
			}

			//! Method must bind class id member to target, which is VBO. Method must be overloaded!
			void bind() const { 
				m_vbo.bind();
			}

			/*
			Method must redefine some or all of data stored in VBO. Method must be overloaded!

				\param vertices - object, which will replace data stored in GPU
			*/
			void update(const std::vector<float>& vertices) const { 
				m_vbo.update(vertices);
			}

			//! Method should reset buffer. Must be overloaded!
			void resetBuffer() { 
				m_vbo.resetBuffer();
			}

			//! Method should unbind currently used VBO and restore memory usage for that buffer. 
			//! Method must be overloaded!
			void unbind() const { 
				m_vbo.unbind();
			}

			//! Method should delete VBO. Method must be overloaded!
			void close() { 
				m_vbo.close();
			}

			/*
			Method should return size (in bytes!), which is allocated on GPU. Method must be overloaded!

				\return size - returns size allocated on GPU
			*/
			int32_t getSize() const { 
				return m_vbo.getSize(); 
			}

			/*
			Returns templated instance of VertexBuffer.

				\return VBO - original instance of VertexBuffer
			*/
			VBO& getInstance() {
				return m_vbo;
			}
		};


} }

#endif // MAR_ENGINE_VERTEXBUFFER_H