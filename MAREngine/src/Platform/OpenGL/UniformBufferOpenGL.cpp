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


	void UniformBufferOpenGL::initialize(const UniformBuffer& uniformBuffer) {
		m_uniformBuffer = uniformBuffer;

		PLATFORM_GL_FUNC( glGenBuffers(1, &m_ubo) );
		PLATFORM_GL_FUNC( glBindBuffer(GL_UNIFORM_BUFFER, m_ubo) );
		PLATFORM_GL_FUNC( glBufferData(GL_UNIFORM_BUFFER, m_uniformBuffer.memory, nullptr, GL_DYNAMIC_DRAW) );
		PLATFORM_GL_FUNC( glBindBufferBase(GL_UNIFORM_BUFFER, m_uniformBuffer.bindingPoint, m_ubo) );
	}

	void UniformBufferOpenGL::close() {
		PLATFORM_GL_FUNC( glDeleteBuffers(1, &m_ubo) );
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


}
