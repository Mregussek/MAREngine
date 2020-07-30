/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "VertexBufferOpenGL.h"


namespace mar {
	namespace graphics {


		void VertexBufferOpenGL::initialize(uint32_t allocationMemory) {
			if (!_initialized) {
				_allocatedMemory = allocationMemory;
				MAR_CORE_GL_FUNC(glGenBuffers(1, &_id));
				MAR_CORE_GL_FUNC(glBindBuffer(GL_ARRAY_BUFFER, _id));
				MAR_CORE_GL_FUNC(glBufferData(GL_ARRAY_BUFFER, _allocatedMemory, nullptr, GL_DYNAMIC_DRAW));
				_initialized = true;
			}
			else {
				GRAPHICS_ERROR("VertexBuffer is already initialized!");
			}
		}

		void VertexBufferOpenGL::bind() const {
			MAR_CORE_GL_FUNC(glBindBuffer(GL_ARRAY_BUFFER, _id));
		}

		void VertexBufferOpenGL::updateDynamically(const std::vector<float>& vertices) const {
			MAR_CORE_GL_FUNC(glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data()));

			GRAPHICS_TRACE("VERTEXBUFFER_OPENGL: updating data!");
		}

		void VertexBufferOpenGL::resetBuffer() {
			MAR_CORE_GL_FUNC(glBufferSubData(GL_ARRAY_BUFFER, 0, 0, nullptr));

			GRAPHICS_TRACE("VERTEXBUFFER_OPENGL: reseting data!");
		}

		void VertexBufferOpenGL::unbind() const {
			MAR_CORE_GL_FUNC(glBindBuffer(GL_ARRAY_BUFFER, 0));
		}

		void VertexBufferOpenGL::close() {
			MAR_CORE_GL_FUNC(glDeleteBuffers(1, &_id));
			_initialized = false;

			GRAPHICS_INFO("VERTEXBUFFER_OPENGL: closed!");
		}

		uint32_t VertexBufferOpenGL::getSize() const {
			int size;
			glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
			return (uint32_t)size;
		}


} }