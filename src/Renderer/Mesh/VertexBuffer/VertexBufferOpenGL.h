/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef VERTEXBUFFER_OPENGL_H
#define VERTEXBUFFER_OPENGL_H

#include "VertexBuffer.h"

namespace mar {
	namespace graphics {


		//! VertexBufferOpenGL
		/*!
			VertexBufferOpenGL is used to store vertices on the GPU memory and as being the source for vertex array data.
			This is overloaded class of base one - VertexBuffer, which implements solution to that problem with OpenGL.
		*/
		class VertexBufferOpenGL : public VertexBuffer, std::enable_shared_from_this<VertexBufferOpenGL> {
			unsigned int _id;
			unsigned int _size;
			unsigned int _allocatedMemory;
			bool _initialized = false;

		public:
			//! Default constructor. For initialization use initializeVertex() method.
			VertexBufferOpenGL() = default;

			//! Initializes VertexBufferObject to m_id and reserves memory on GPU, also tells
			//! the GPU that user will change resources dynamically.
			/*
				\param allocationMemory - size of memory (bytes!), which will be allocated on GPU
			*/
			void initializeVertex(unsigned int allocationMemory) override;

			//! Binds class m_id member to target, which is VBO
			void bind() const override;

			//! Redefines some or all of data stored in VBO.
			/*
				\param vertices - object, which will replace data stored in GPU
			*/
			void updateDynamically(const std::vector<float>& vertices) const override;

			//! Method resets buffer.
			void resetBuffer() override;

			//! Unbinds currently used VBO and restore memory usage for that buffer
			void unbind() const override;

			//! Deletes VBO
			void close() override;

			//! Get size (in bytes!), which is allocated on GPU
			/*
				\return size - returns size allocated on GPU
			*/
			unsigned int getSize() const override;
		};


} }

#endif // VERTEXBUFFER_OPENGL_H