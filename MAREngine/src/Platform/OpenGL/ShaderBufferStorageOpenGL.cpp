/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#include "ShaderBufferStorageOpenGL.h"


namespace marengine {


	void ShaderBufferStorageOpenGL::initialize(const UniformBuffer& uniformBuffer, const std::vector<UniformItem>& items) {
		m_uniformBuffer = uniformBuffer;
		m_uniformItems = items;

		PLATFORM_TRACE("SHADER_STORAGE_BUFFER_OPENGL: creating {} ...", m_uniformBuffer.name);

		PLATFORM_GL_FUNC( glGenBuffers(1, &m_ssbo) );
		PLATFORM_GL_FUNC( glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo) );
		PLATFORM_GL_FUNC( glBufferData(GL_SHADER_STORAGE_BUFFER, m_uniformBuffer.memory, nullptr, GL_DYNAMIC_DRAW) );
		PLATFORM_GL_FUNC( glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_uniformBuffer.bindingPoint, m_ssbo) );

		PLATFORM_INFO("SHADER_STORAGE_BUFFER_OPENGL: created {} with ID = {}", m_uniformBuffer.name, m_ssbo);
	}

	void ShaderBufferStorageOpenGL::close() {
		PLATFORM_TRACE("SHADER_STORAGE_BUFFER_OPENGL: deleting {}, ID = {} ...", m_uniformBuffer.name, m_ssbo);

		m_uniformItems.clear();
		PLATFORM_GL_FUNC( glDeleteBuffers(1, &m_ssbo) );

		PLATFORM_INFO("SHADER_STORAGE_BUFFER_OPENGL: deleted {}", m_uniformBuffer.name);
	}

	void ShaderBufferStorageOpenGL::reset() const {
		PLATFORM_GL_FUNC( glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, 0, nullptr) );
	}

	void ShaderBufferStorageOpenGL::bind() const {
		PLATFORM_TRACE("SHADER_STORAGE_BUFFER_OPENGL: binding {}, ID = {}...", m_uniformBuffer.name, m_ssbo);

		PLATFORM_GL_FUNC( glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo) );
	}

	void ShaderBufferStorageOpenGL::unbind() const {
		PLATFORM_TRACE("SHADER_STORAGE_BUFFER_OPENGL: unbinding {}, ID = {}...", m_uniformBuffer.name, m_ssbo);

		PLATFORM_GL_FUNC( glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0) );
	}


}
