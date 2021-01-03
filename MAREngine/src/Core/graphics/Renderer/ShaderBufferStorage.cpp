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


#include "ShaderBufferStorage.h"
#include "../GraphicsLogs.h"
#include "../../../Platform/GLSL/ShaderUniforms.h"


namespace marengine {


	ShaderBufferStorage* ShaderBufferStorage::Instance{ nullptr };


	ShaderBufferStorageOpenGL& ShaderBufferStorage::createShaderBufferStorage() {
		GRAPHICS_TRACE("SHADER_BUFFER_STORAGE: creating SSBO at index {}..., currentSSBOsize = {}", m_shaderBuffers.size(), m_shaderBuffers.size() + 1);

		return m_shaderBuffers.emplace_back();
	}

	UniformBufferOpenGL& ShaderBufferStorage::createUniformBufferObject() {
		GRAPHICS_TRACE("SHADER_BUFFER_STORAGE: creating UBO at index {}..., currentUBOsize = {}", m_uniformBuffers.size(), m_uniformBuffers.size() + 1);

		return m_uniformBuffers.emplace_back();
	}

	const ShaderBufferStorageOpenGL& ShaderBufferStorage::getCorrectShaderBuffer(const UniformBuffer& buffer) const {
		return *std::find_if(m_shaderBuffers.cbegin(), m_shaderBuffers.cend(), [&buffer](const ShaderBufferStorageOpenGL& ubo) {
			return std::strcmp(ubo.m_uniformBuffer.name, buffer.name) == 0;
		});
	}

	const UniformBufferOpenGL& ShaderBufferStorage::getCorrectUniformBuffer(const UniformBuffer& buffer) const {
		return *std::find_if(m_uniformBuffers.cbegin(), m_uniformBuffers.cend(), [&buffer](const UniformBufferOpenGL& ubo) {
			return std::strcmp(ubo.m_uniformBuffer.name, buffer.name) == 0;
		});
	}

	const std::vector<ShaderBufferStorageOpenGL>& ShaderBufferStorage::getSSBOs() const {
		return m_shaderBuffers;
	}

	const std::vector<UniformBufferOpenGL>& ShaderBufferStorage::getUBOs() const {
		return m_uniformBuffers;
	}

	ShaderBufferStorageOpenGL& ShaderBufferStorage::getSSBO(uint32_t index) {
		GRAPHICS_TRACE("SHADER_BUFFER_STORAGE: returning SSBO at index = {}...", index);

		return m_shaderBuffers.at(index);
	}

	UniformBufferOpenGL& ShaderBufferStorage::getUBO(uint32_t index) {
		GRAPHICS_TRACE("SHADER_BUFFER_STORAGE: returning UBO at index = {}...", index);

		return m_uniformBuffers.at(index);
	}

	void ShaderBufferStorage::close() {
		GRAPHICS_TRACE("SHADER_BUFFER_STORAGE: closing...");

		std::for_each(m_shaderBuffers.begin(), m_shaderBuffers.end(), [](ShaderBufferStorageOpenGL& shaderBuffer) {
			shaderBuffer.close();
		});

		std::for_each(m_uniformBuffers.begin(), m_uniformBuffers.end(), [](UniformBufferOpenGL& uniformBuffer) {
			uniformBuffer.close();
		});

		m_shaderBuffers.clear();
		m_uniformBuffers.clear();

		GRAPHICS_INFO("SHADER_BUFFER_STORAGE: closed!");
	}


}
