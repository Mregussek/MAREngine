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
				MAR_CORE_GL_FUNC( glGenBuffers(1, &_id) );
				MAR_CORE_GL_FUNC( glBindBuffer(GL_ARRAY_BUFFER, _id) );
				MAR_CORE_GL_FUNC( glBufferData(GL_ARRAY_BUFFER, _allocatedMemory, nullptr, GL_DYNAMIC_DRAW) );
				_initialized = true;

				GRAPHICS_INFO("VERTEXBUFFER_OPENGL: VBO - {} initialized!", _id);
			}
			else {
				GRAPHICS_ERROR("VERTEXBUFFER_OPENGL: VBO - {} is already initialized!", _id);
			}
		}

		void VertexBufferOpenGL::bind() const {
			MAR_CORE_GL_FUNC( glBindBuffer(GL_ARRAY_BUFFER, _id) );

			GRAPHICS_TRACE("VERTEXBUFFER_OPENGL: Binding VBO - {}!", _id);
		}

		void VertexBufferOpenGL::update(const std::vector<float>& vertices) const {
			MAR_CORE_GL_FUNC( glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data()) );

			GRAPHICS_TRACE("VERTEXBUFFER_OPENGL: updating data in VBO - {}! Num of vertices: {}, Memory: {}", _id, vertices.size(), vertices.size() * sizeof(float));
		}

		void VertexBufferOpenGL::resetBuffer() {
			MAR_CORE_GL_FUNC(glBufferSubData(GL_ARRAY_BUFFER, 0, 0, nullptr));

			GRAPHICS_TRACE("VERTEXBUFFER_OPENGL: reseting data - {}!", _id);
		}

		void VertexBufferOpenGL::unbind() const {
			MAR_CORE_GL_FUNC(glBindBuffer(GL_ARRAY_BUFFER, 0));

			GRAPHICS_TRACE("VERTEXBUFFER_OPENGL: unbiding - {}!", _id);
		}

		void VertexBufferOpenGL::close() {
			GRAPHICS_INFO("VERTEXBUFFER_OPENGL: closing VBO - {}!", _id);

			MAR_CORE_GL_FUNC(glDeleteBuffers(1, &_id));
			_initialized = false;
		}

		int32_t VertexBufferOpenGL::getSize() const {
			int32_t size;
			glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
			return size;
		}


} }