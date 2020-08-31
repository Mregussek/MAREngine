/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "ElementBufferOpenGL.h"


namespace mar {
	namespace platforms {


		void ElementBufferOpenGL::initialize(const uint32_t allocationMemory) {
			if (!_initialized) {
				_allocatedMemory = allocationMemory;
				PLATFORM_GL_FUNC( glGenBuffers(1, &_id) );
				PLATFORM_GL_FUNC( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id) );
				PLATFORM_GL_FUNC( glBufferData(GL_ELEMENT_ARRAY_BUFFER, _allocatedMemory, nullptr, GL_DYNAMIC_DRAW) );
				_initialized = true;
			}
			else {
				PLATFORM_ERROR("ELEMENTBUFFER_OPENGL: cannot re-initialize EBO! - {}", _id);
			}
		}

		void ElementBufferOpenGL::bind() const {
			PLATFORM_GL_FUNC( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id) );

			PLATFORM_TRACE("ELEMENTBUFFER_OPENGL: Binding EBO - {}!", _id);
		}

		void ElementBufferOpenGL::update(const std::vector<uint32_t>& data) const {
			PLATFORM_GL_FUNC( glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, data.size() * sizeof(uint32_t), data.data()) );

			PLATFORM_TRACE("ELEMENTBUFFER_OPENGL: updating data in EBO - {}! Num of indices: {}, Memory: {}", _id, data.size(), data.size() * sizeof(uint32_t));
		}

		void ElementBufferOpenGL::resetBuffer() {
			PLATFORM_GL_FUNC( glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 0, nullptr) );

			PLATFORM_TRACE("ELEMENTBUFFER_OPENGL: reseting data in EBO - {}!", _id);
		}

		void ElementBufferOpenGL::unbind() const {
			PLATFORM_GL_FUNC( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );

			PLATFORM_TRACE("ELEMENTBUFFER_OPENGL: Unbinding EBO - {}!", _id);
		}

		void ElementBufferOpenGL::close() {
			if (_initialized) {
				PLATFORM_TRACE("ELEMENTBUFFER_OPENGL: deleting {}!", _id);

				PLATFORM_GL_FUNC( glDeleteBuffers(1, &_id) );
				_initialized = false;
			}
		}


} }
