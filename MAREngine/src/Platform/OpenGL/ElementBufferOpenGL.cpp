/**
 *				MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


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
