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


#ifndef MAR_ENGINE_PLATFORMS_SHADER_BUFFER_STORAGE_OPENGL_H
#define MAR_ENGINE_PLATFORMS_SHADER_BUFFER_STORAGE_OPENGL_H


#include "../../mar.h"
#include "../PlatformLogs.h"
#include "../GLSL/ShaderUniforms.h"


namespace marengine {

	class ShaderBufferStorage;


	class ShaderBufferStorageOpenGL {

		friend class ShaderBufferStorage;

	public:

		void initialize(const UniformBuffer& uniformBuffer, const std::vector<UniformItem>& items);
		void close();

		void bind() const;
		void unbind() const;

		template<typename T>
		void update(uint32_t offset, uint32_t memory, const std::vector<T>& data) const {
			PLATFORM_TRACE("SHADER_STORAGE_BUFFER_OPENGL: updating SSBO {} with vector<{}>, offset = {}, memory = {}, data.size() = {}",
				m_ssbo, typeid(T).name(), offset, memory, data.size());

			PLATFORM_GL_FUNC( glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, memory, data.data()) );
		}

		template<typename T>
		void update(uint32_t offset, uint32_t memory, const T* data) const {
			PLATFORM_TRACE("SHADER_STORAGE_BUFFER_OPENGL: updating SSBO {} with const {}*, offset = {}, memory = {}",
				m_ssbo, typeid(T).name(), offset, memory);

			PLATFORM_GL_FUNC( glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, memory, data) );
		}

		void reset() const;

	private:

		UniformBuffer m_uniformBuffer{ "Null", 0, 0 };
		std::vector<UniformItem> m_uniformItems;
		uint32_t m_ssbo{ 0 };

	};


}


#endif // !MAR_ENGINE_PLATFORMS_SHADER_BUFFER_STORAGE_OPENGL_H
