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


#ifndef MAR_ENGINE_PLATFORMS_SHADER_BUFFER_STORAGE_OPENGL_H
#define MAR_ENGINE_PLATFORMS_SHADER_BUFFER_STORAGE_OPENGL_H


#include "../../mar.h"
#include "../PlatformLogs.h"
#include "ShaderUniforms.h"


namespace mar::graphics { class ShaderBufferStorage; }
namespace mar::platforms {


	class ShaderBufferStorageOpenGL {

		friend class graphics::ShaderBufferStorage;

		typedef ShaderUniforms::UniformBuffer UniformBuffer;
		typedef ShaderUniforms::UniformItem UniformItem;

	public:

		void initialize(const UniformBuffer& uniformBuffer, std::vector<UniformItem>&& items);
		void close();

		void bind() const;
		void unbind() const;

		template<typename T>
		void update(uint32_t offset, uint32_t memory, const std::vector<T>& data) const {
			PLATFORM_GL_FUNC( glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, memory, data.data()) );
		}

		template<typename T>
		void update(uint32_t offset, uint32_t memory, const T* data) const {
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
