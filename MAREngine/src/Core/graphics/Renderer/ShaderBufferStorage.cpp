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


namespace mar::graphics {


	platforms::ShaderBufferStorageOpenGL& ShaderBufferStorage::createShaderBufferStorage() {
		return m_shaderBuffers.emplace_back();
	}

	platforms::UniformBufferOpenGL& ShaderBufferStorage::createUniformBufferObject() {
		return m_uniformBuffers.emplace_back();
	}

	const platforms::ShaderBufferStorageOpenGL& ShaderBufferStorage::getCorrectShaderBuffer(const UniformBuffer& buffer) const {
		return *std::find_if(m_shaderBuffers.cbegin(), m_shaderBuffers.cend(), [&buffer](const ShaderBufferStorageOpenGL& ubo) {
			return std::strcmp(ubo.m_uniformBuffer.name, buffer.name) == 0;
		});
	}

	const platforms::UniformBufferOpenGL& ShaderBufferStorage::getCorrectUniformBuffer(const UniformBuffer& buffer) const {
		return *std::find_if(m_uniformBuffers.cbegin(), m_uniformBuffers.cend(), [&buffer](const UniformBufferOpenGL& ubo) {
			return std::strcmp(ubo.m_uniformBuffer.name, buffer.name) == 0;
		});
	}

	void ShaderBufferStorage::close() {
		std::for_each(m_shaderBuffers.begin(), m_shaderBuffers.end(), [](ShaderBufferStorageOpenGL& shaderBuffer) {
			shaderBuffer.close();
		});

		std::for_each(m_uniformBuffers.begin(), m_uniformBuffers.end(), [](UniformBufferOpenGL& uniformBuffer) {
			uniformBuffer.close();
		});

		m_shaderBuffers.clear();
		m_uniformBuffers.clear();
	}


}
