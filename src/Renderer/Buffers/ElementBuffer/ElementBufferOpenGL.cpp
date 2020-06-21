/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "ElementBufferOpenGL.h"

namespace mar {
	namespace graphics {


		void ElementBufferOpenGL::initializeElement(const unsigned int allocationMemory) {
			if (!_initialized) {
				_allocatedMemory = allocationMemory;
				glGenBuffers(1, &_id);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, _allocatedMemory, nullptr, GL_DYNAMIC_DRAW);
				_initialized = true;
			}
			else {
				std::cerr << "ElementBuffer is already initialized!\n";
			}
		}

		void ElementBufferOpenGL::bind() const {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
		}

		void ElementBufferOpenGL::updateDynamically(const std::vector<unsigned int>& data) const {
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, data.size() * sizeof(unsigned int), data.data());
		}

		void ElementBufferOpenGL::resetBuffer() {
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 0, nullptr);
		}

		void ElementBufferOpenGL::unbind() const {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		void ElementBufferOpenGL::close() {
			if (_initialized) {
				glDeleteBuffers(1, &_id);
				_initialized = false;
			}
		}


} }
