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


#ifndef MAR_ENGINE_PLATFORMS_UNIFORM_BUFFER_OPENGL_H
#define MAR_ENGINE_PLATFORMS_UNIFORM_BUFFER_OPENGL_H


#include "../../mar.h"
#include "../PlatformLogs.h"
#include "ShaderUniforms.h"


namespace mar::platforms {


	class UniformBufferOpenGL {

		friend class ShaderOpenGL;

		typedef ShaderUniforms::UniformBuffer UniformBuffer;
		typedef ShaderUniforms::UniformItem UniformItem;

	public:

		void initialize(const UniformBuffer& uniformBuffer);
		void close();

		void bind() const;
		void unbind() const;

		template<typename T>
		void update(uint32_t offset, uint32_t memory, const std::vector<T>& data) const {
			PLATFORM_GL_FUNC( glBufferSubData(GL_UNIFORM_BUFFER, offset, memory, data.data()) );
		}

		template<typename T>
		void update(uint32_t offset, uint32_t memory, const T* data) const {
			PLATFORM_GL_FUNC( glBufferSubData(GL_UNIFORM_BUFFER, offset, memory, data) );
		}

		void reset() const;

	private:

		UniformBuffer m_uniformBuffer{ "Null", 0, 0 };
		uint32_t m_ubo{ 0 };

	};


}


#endif // !MAR_ENGINE_PLATFORMS_UNIFORM_BUFFER_OPENGL_H
