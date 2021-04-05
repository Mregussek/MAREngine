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


#include "Renderer.h"
#include "RendererEvents.h"
#include "PipelineManager.h"
#include "RenderMemorizer.h"
#include "../RenderAPI/RenderPipeline.h"
#include "../Mesh/MeshBatchStatic.h"
#include "../Lightning/PointLightBatch.h"
#include "../../../Platform/OpenGL/DrawingOpenGL.h"
#include "../../../Platform/OpenGL/TextureOpenGL.h"


namespace marengine {


	void FRenderer::initialize() {
		setupShaders();
	}

	void FRenderer::setupShaders() {
		{
			const char* vert = "resources/shaders/color.vert.glsl";
			const char* frag = "resources/shaders/color.frag.glsl";
			const ShaderPaths shaderPaths(vert, frag, nullptr);

			m_shaderColors.initialize(shaderPaths);
		}
		{
			const char* vert = "resources/shaders/texture2d.vert.glsl";
			const char* frag = "resources/shaders/texture2d.frag.glsl";
			const ShaderPaths shaderPaths(vert, frag, nullptr);

			m_shader2D.initialize(shaderPaths);
			m_shader2D.setupShaderUniforms(GLSLShaderInfo::samplerTexture2DArray);
		}
	}

	void FRenderer::close() {
		m_shaderColors.shutdown();
		m_shader2D.shutdown();
		TextureOpenGL::Instance()->shutdown();
	}

	void FRenderer::draw() const {
		const RenderPipeline* renderPipeline( RenderPipeline::Instance );

		const auto& cameraSSBO{ FPipelineManager::Instance->getSSBO(RenderMemorizer::Instance->cameraSSBO) };
		cameraSSBO.bind();

		const auto& pointLightBatch{ renderPipeline->getPointLightBatch() };
		const auto& pointLightSSBO{ FPipelineManager::Instance->getSSBO(pointLightBatch.getUniquePointLightID()) };
		pointLightSSBO.bind();
		
		m_shaderColors.bind();
		drawColors(renderPipeline->getColorBatches()[0]);
		m_shader2D.bind();
		drawTextures2D(renderPipeline->getTexture2DBatches()[0]);
	}

	void FRenderer::drawColors(const FMeshBatchStaticColor& batch) const {
		const auto& transformSSBO{ FPipelineManager::Instance->getSSBO(batch.getUniqueTransformsID()) };
		transformSSBO.bind();

		const auto& colorSSBO{ FPipelineManager::Instance->getSSBO(batch.getUniqueColorsID()) };
		colorSSBO.bind();

		const auto& pipeline{ FPipelineManager::Instance->getPipeline(batch.getUniquePipelineID()) };
		pipeline.bind();

		DrawingOpenGL::drawTriangles(batch.getIndices().size());
		FRendererEvents::onDrawCall();
	}

	void FRenderer::drawTextures2D(const FMeshBatchStaticTexture2D& batch) const {
		const auto& transformSSBO{ FPipelineManager::Instance->getSSBO(batch.getUniqueTransformsID()) };
		transformSSBO.bind();

		const auto& textures{ batch.getTextures() };
		std::for_each(textures.cbegin(), textures.cend(), [this](const FTexturePair& texture) {
			const auto textureID = (int32_t)TextureOpenGL::Instance()->loadTexture(texture.texturePath);
			TextureOpenGL::Instance()->bind2D(texture.bindingIndex, textureID);
			m_shader2D.setUniformSampler(GLSLShaderInfo::samplerTexture2DArray[texture.bindingIndex], texture.bindingIndex);
		});

		const auto& pipeline{ FPipelineManager::Instance->getPipeline(batch.getUniquePipelineID()) };
		pipeline.bind();

		DrawingOpenGL::drawTriangles(batch.getIndices().size());
		FRendererEvents::onDrawCall();
	}


}
