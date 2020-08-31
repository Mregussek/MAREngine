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
				PLATFORM_GL_FUNC( glGenBuffers(1, &_id) );
				PLATFORM_GL_FUNC( glBindBuffer(GL_ARRAY_BUFFER, _id) );
				PLATFORM_GL_FUNC( glBufferData(GL_ARRAY_BUFFER, _allocatedMemory, nullptr, GL_DYNAMIC_DRAW) );
				_initialized = true;

				PLATFORM_INFO("VERTEXBUFFER_OPENGL: VBO - {} initialized!", _id);
			}
			else {
				PLATFORM_ERROR("VERTEXBUFFER_OPENGL: VBO - {} is already initialized!", _id);
			}
		}

		void VertexBufferOpenGL::bind() const {
			PLATFORM_GL_FUNC( glBindBuffer(GL_ARRAY_BUFFER, _id) );

			PLATFORM_TRACE("VERTEXBUFFER_OPENGL: Binding VBO - {}!", _id);
		}

		void VertexBufferOpenGL::update(const std::vector<float>& vertices) const {
			PLATFORM_GL_FUNC( glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data()) );

			PLATFORM_TRACE("VERTEXBUFFER_OPENGL: updating data in VBO - {}! Num of vertices: {}, Memory: {}", _id, vertices.size(), vertices.size() * sizeof(float));
		}

		void VertexBufferOpenGL::resetBuffer() {
			PLATFORM_GL_FUNC(glBufferSubData(GL_ARRAY_BUFFER, 0, 0, nullptr));

			PLATFORM_TRACE("VERTEXBUFFER_OPENGL: reseting data - {}!", _id);
		}

		void VertexBufferOpenGL::unbind() const {
			PLATFORM_GL_FUNC(glBindBuffer(GL_ARRAY_BUFFER, 0));

			PLATFORM_TRACE("VERTEXBUFFER_OPENGL: unbiding - {}!", _id);
		}

		void VertexBufferOpenGL::close() {
			PLATFORM_INFO("VERTEXBUFFER_OPENGL: closing VBO - {}!", _id);

			PLATFORM_GL_FUNC(glDeleteBuffers(1, &_id));
			_initialized = false;
		}

		int32_t VertexBufferOpenGL::getSize() const {
			int32_t size;
			glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
			return size;
		}


} }