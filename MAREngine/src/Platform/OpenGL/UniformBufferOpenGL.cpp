/**
 *           MAREngine - open source 3D game engine
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


#include "UniformBufferOpenGL.h"


namespace mar::platforms {


	void UniformBufferOpenGL::initialize(const char* name, uint64_t memoryToAllocate) {
		m_name = name;

		PLATFORM_GL_FUNC( glGenBuffers(1, &m_ubo) );
		PLATFORM_GL_FUNC( glBindBuffer(GL_UNIFORM_BUFFER, m_ubo) );
		PLATFORM_GL_FUNC( glBufferData(GL_UNIFORM_BUFFER, memoryToAllocate, nullptr, GL_DYNAMIC_DRAW) );
	}

	void UniformBufferOpenGL::close() {
		PLATFORM_GL_FUNC( glDeleteBuffers(1, &m_ubo) );
	}

	template<typename T>
	void UniformBufferOpenGL::update(uint64_t offset, uint64_t memory, const std::vector<T>& data) const {
		PLATFORM_GL_FUNC( glBufferSubData(GL_UNIFORM_BUFFER, offset, memory, data.data()) );
	}

	void UniformBufferOpenGL::reset() const {
		PLATFORM_GL_FUNC( glBufferSubData(GL_UNIFORM_BUFFER, 0, 0, nullptr) );
	}

	void UniformBufferOpenGL::bind() const {
		PLATFORM_GL_FUNC( glBindBuffer(GL_UNIFORM_BUFFER, m_ubo) );
	}

	void UniformBufferOpenGL::unbind() const {
		PLATFORM_GL_FUNC( glBindBuffer(GL_UNIFORM_BUFFER, 0) );
	}

	void UniformBufferOpenGL::bindToLayout(uint32_t bindingPoint) const {
		PLATFORM_GL_FUNC( glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_ubo) );
	}


}
