/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef ELEMENTBUFFER_OPENGL_H
#define ELEMENTBUFFER_OPENGL_H

#include "../../mar.h"
#include "../PlatformLogs.h"


namespace mar {
	namespace graphics {


		/*!
			ElementBufferOpenGL

			ElementBufferOpenGL is used to store indices in GPU memory and dynamically set new values if needed.
			Other common name for ElementBuffer is IndexBuffer.
			This is overloaded class of base one - ElementBuffer, which implements solution to that problem with OpenGL.
		*/
		class ElementBufferOpenGL {
			uint32_t _id;
			uint32_t _allocatedMemory;
			bool _initialized = false;

		public:
			//! Default constructor. For initialization use initialize() method.
			ElementBufferOpenGL() = default;

			/*
			Initializes ElementBufferObject to m_id, defines data on GPU, reserves memory on GPU,
			also tells the GPU that user will change resources dynamically.

				\param allocationMemory - memory (in bytes!), which will be allocated in GPU
			*/
			void initialize(const uint32_t allocationMemory);

			/*
			Redefines some or all of data stored in EBO.

				\param data - indices, which will be drawn
			*/
			void update(const std::vector<uint32_t>& data) const;

			//! Method resets buffer.
			void resetBuffer();

			//! Binds class m_id member to target, which is EBO
			void bind() const;

			//! Unbind currently used EBO and restore memory usage for that buffer
			void unbind() const;

			//! Deletes EBO
			void close();
		};


} }


#endif // !ELEMENTBUFFER_OPENGL_H