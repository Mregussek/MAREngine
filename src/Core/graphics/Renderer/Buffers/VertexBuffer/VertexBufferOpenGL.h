/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_VERTEXBUFFER_OPENGL_H
#define MAR_ENGINE_VERTEXBUFFER_OPENGL_H

#include "VertexBuffer.h"


namespace mar {
	namespace graphics {


		/*!
		VertexBufferOpenGL

			VertexBufferOpenGL is used to store vertices on the GPU memory and as being the source for vertex array data.
			This is overloaded class of base one - VertexBuffer, which implements solution to that problem with OpenGL.
		*/
		class VertexBufferOpenGL {
			uint32_t _id;
			uint32_t _size;
			uint32_t _allocatedMemory;
			bool _initialized = false;

		public:
			//! Default constructor. For initialization use initialize() method.
			VertexBufferOpenGL() = default;

			/*
			Initializes VertexBufferObject to m_id and reserves memory on GPU, also tells
			the GPU that user will change resources dynamically.

				\param allocationMemory - size of memory (bytes!), which will be allocated on GPU
			*/
			void initialize(uint32_t allocationMemory);

			//! Binds class m_id member to target, which is VBO
			void bind() const;

			/*
			Redefines some or all of data stored in VBO.

				\param vertices - object, which will replace data stored in GPU
			*/
			void update(const std::vector<float>& vertices) const;

			//! Method resets buffer.
			void resetBuffer();

			//! Unbinds currently used VBO and restore memory usage for that buffer
			void unbind() const;

			//! Deletes VBO
			void close();

			/*
			Get size (in bytes!), which is allocated on GPU

				\return size - returns size allocated on GPU
			*/
			int32_t getSize() const;
		};


} }

#endif // MAR_ENGINE_VERTEXBUFFER_OPENGL_H