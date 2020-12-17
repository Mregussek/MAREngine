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


#ifndef MAR_ENGINE_GRAPHICS_SHADER_BUFFER_STORAGE_H
#define MAR_ENGINE_GRAPHICS_SHADER_BUFFER_STORAGE_H


#include "../../../mar.h"
#include "../../../Platform/OpenGL/ShaderBufferStorageOpenGL.h"
#include "../../../Platform/OpenGL/UniformBufferOpenGL.h"


namespace mar::graphics {


	class ShaderBufferStorage {

		typedef platforms::UniformBuffer UniformBuffer;
		typedef platforms::ShaderBufferStorageOpenGL ShaderBufferStorageOpenGL;
		typedef platforms::UniformBufferOpenGL UniformBufferOpenGL;

	public:

		static ShaderBufferStorage* Instance;

		ShaderBufferStorageOpenGL& createShaderBufferStorage();
		UniformBufferOpenGL& createUniformBufferObject();

		const ShaderBufferStorageOpenGL& getCorrectShaderBuffer(const UniformBuffer& buffer) const;
		const UniformBufferOpenGL& getCorrectUniformBuffer(const UniformBuffer& buffer) const;

		const std::vector<ShaderBufferStorageOpenGL>& getSSBOs() const;
		const std::vector<UniformBufferOpenGL>& getUBOs() const;

		ShaderBufferStorageOpenGL& getSSBO(uint32_t index);
		UniformBufferOpenGL& getUBO(uint32_t index);

		void close();

	private:

		std::vector<ShaderBufferStorageOpenGL> m_shaderBuffers;
		std::vector<UniformBufferOpenGL> m_uniformBuffers;

	};


}


#endif // !MAR_ENGINE_GRAPHICS_SHADER_BUFFER_STORAGE_H
