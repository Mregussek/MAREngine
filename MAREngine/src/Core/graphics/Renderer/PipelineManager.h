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


#ifndef MAR_ENGINE_GRAPHICS_RENDERER_PIPELINE_STORAGE_H
#define MAR_ENGINE_GRAPHICS_RENDERER_PIPELINE_STORAGE_H


#include "../../../Platform/OpenGL/PipelineOpenGL.h"
#include "../../../Platform/OpenGL/ShaderStorageBufferOpenGL.h"
#include "../../../Platform/OpenGL/UniformBufferOpenGL.h"


namespace marengine {
	

	class FPipelineManager {
	public:

		static FPipelineManager* Instance;

		PipelineOpenGL& createPipeline();
		FShaderStorageBufferOpenGL& createSSBO();
		FUniformBufferOpenGL& createUBO();

		const std::vector<PipelineOpenGL>& getPipelines() const;
		const std::vector<FShaderStorageBufferOpenGL>& getSSBOs() const;
		const std::vector<FUniformBufferOpenGL>& getUBOs() const;

		const PipelineOpenGL& getPipeline(uint32_t index) const;
		const FShaderStorageBufferOpenGL& getSSBO(uint32_t index) const;
		const FUniformBufferOpenGL& getUBO(uint32_t index) const;

		void close();

	private:

		std::vector<PipelineOpenGL> m_pipelines;
		std::vector<FShaderStorageBufferOpenGL> m_shaderStorageBuffers;
        std::vector<FUniformBufferOpenGL> m_uniformBuffers;

	};


}


#endif // !MAR_ENGINE_GRAPHICS_RENDERER_PIPELINE_STORAGE_H
