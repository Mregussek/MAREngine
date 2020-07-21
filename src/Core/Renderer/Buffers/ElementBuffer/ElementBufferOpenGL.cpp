/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "ElementBufferOpenGL.h"


namespace mar {
	namespace graphics {


		void ElementBufferOpenGL::initialize(const uint32_t allocationMemory) {
			if (!_initialized) {
				_allocatedMemory = allocationMemory;
				MAR_CORE_GL_FUNC( glGenBuffers(1, &_id) );
				MAR_CORE_GL_FUNC( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id) );
				MAR_CORE_GL_FUNC( glBufferData(GL_ELEMENT_ARRAY_BUFFER, _allocatedMemory, nullptr, GL_DYNAMIC_DRAW) );
				_initialized = true;
			}
			else {
				std::cerr << "ElementBuffer is already initialized!\n";
			}
		}

		void ElementBufferOpenGL::bind() const {
			MAR_CORE_GL_FUNC( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id) );
		}

		void ElementBufferOpenGL::update(const std::vector<uint32_t>& data) const {
			MAR_CORE_GL_FUNC( glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, data.size() * sizeof(uint32_t), data.data()) );
		}

		void ElementBufferOpenGL::resetBuffer() {
			MAR_CORE_GL_FUNC( glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 0, nullptr) );
		}

		void ElementBufferOpenGL::unbind() const {
			MAR_CORE_GL_FUNC( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
		}

		void ElementBufferOpenGL::close() {
			if (_initialized) {
				MAR_CORE_GL_FUNC( glDeleteBuffers(1, &_id) );
				_initialized = false;
			}
		}


} }
