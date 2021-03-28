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


#include "PipelineManager.h"


namespace marengine {


	FPipelineManager* FPipelineManager::Instance{ nullptr };


    PipelineOpenGL& FPipelineManager::createPipeline() {
		return m_pipelines.emplace_back();
	}

    FShaderStorageBufferOpenGL& FPipelineManager::createSSBO() {
		return m_shaderStorageBuffers.emplace_back();
	}

    FUniformBufferOpenGL& FPipelineManager::createUBO() {
		return m_uniformBuffers.emplace_back();
	}

	const std::vector<PipelineOpenGL>& FPipelineManager::getPipelines() const {
		return m_pipelines;
	}

	const std::vector<FShaderStorageBufferOpenGL>& FPipelineManager::getSSBOs() const {
		return m_shaderStorageBuffers;
	}

	const std::vector<FUniformBufferOpenGL>& FPipelineManager::getUBOs() const {
		return m_uniformBuffers;
	}

	const PipelineOpenGL& FPipelineManager::getPipeline(uint32_t index) const {
		return m_pipelines.at(index);
	}

	const FShaderStorageBufferOpenGL& FPipelineManager::getSSBO(uint32_t index) const {
		return m_shaderStorageBuffers.at(index);
	}

	const FUniformBufferOpenGL& FPipelineManager::getUBO(uint32_t index) const {
		return m_uniformBuffers.at(index);
	}

	void FPipelineManager::close() {
		for (auto& pipeline : m_pipelines) {
			pipeline.close();
		}
		for (auto& ssbo : m_shaderStorageBuffers) {
			ssbo.close();
		}
		for (auto& ubo : m_uniformBuffers) {
			ubo.close();
		}

		m_pipelines.clear();
		m_shaderStorageBuffers.clear();
		m_uniformBuffers.clear();
	}


}
