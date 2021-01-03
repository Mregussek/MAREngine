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


#include "RendererBatch.h"
#include "RendererEvents.h"
#include "ShaderBufferStorage.h"
#include "../RenderAPI/RenderMemorizer.h"
#include "PipelineStorage.h"
#include "../GraphicsLogs.h"
#include "../../../Platform/OpenGL/DrawingOpenGL.h"
#include "../../../Platform/OpenGL/TextureOpenGL.h"
#include "../../../Platform/GLSL/ShaderUniforms.h"

#include "../Mesh/MeshBatchStaticColor.h"
#include "../Mesh/MeshBatchStaticTexture2D.h"
#include "../Lightning/PointLightBatch.h"


namespace marengine {


	void RendererBatch::initialize() {
		GRAPHICS_INFO("RENDERER_BATCH: going to initialize!");

		const char* vert = "resources/shaders/default.vert.glsl";

		{
			const char* frag = "resources/shaders/color.frag.glsl";
			const ShaderPaths shaderPaths(vert, frag, nullptr);

			m_shaderColors.initialize(shaderPaths);
		}
		{
			const char* frag = "resources/shaders/texture2d.frag.glsl";
			const ShaderPaths shaderPaths(vert, frag, nullptr);

			m_shaderTexture2D.initialize(shaderPaths);
			m_shaderTexture2D.setupShaderUniforms(GLSLShaderInfo::samplerTexture2DArray);
		}
	}

	void RendererBatch::close() {
		GRAPHICS_INFO("RENDERER_BATCH: going to close!");

		m_shaderColors.shutdown();
		m_shaderTexture2D.shutdown();

		GRAPHICS_INFO("RENDERER_BATCH: closed!");
	}

	void RendererBatch::draw(const FMeshBatchStaticColor& staticColorBatch, const FPointLightBatch& pointLightBatch) const {
		m_shaderColors.bind();

		const auto& cameraSSBO{ ShaderBufferStorage::Instance->getSSBO(RenderMemorizer::Instance->cameraSSBO) };
		cameraSSBO.bind();

		const auto& batchPipeline{ PipelineStorage::Instance->getPipeline(staticColorBatch.getUniquePipelineID()) };
		batchPipeline.bind();

		const auto& transformsSSBO{ ShaderBufferStorage::Instance->getSSBO(staticColorBatch.getTransformsSSBOindex()) };
		transformsSSBO.bind();

		const auto& colorsSSBO{ ShaderBufferStorage::Instance->getSSBO(staticColorBatch.getColorsSSBOindex()) };
		colorsSSBO.bind();

		const auto& pointLightSSBO{ ShaderBufferStorage::Instance->getSSBO(pointLightBatch.getPointLightSSBOindex()) };
		pointLightSSBO.bind();

		DrawingOpenGL::drawTriangles(staticColorBatch.getIndices().size());
		FRendererEvents::onDrawCall();
	}

	void RendererBatch::draw(const FMeshBatchStaticTexture2D& staticTexture2DBatch, const FPointLightBatch& pointLightBatch) const {
		m_shaderTexture2D.bind();

		const auto& cameraSSBO{ ShaderBufferStorage::Instance->getSSBO(RenderMemorizer::Instance->cameraSSBO) };
		cameraSSBO.bind();

		const auto& batchPipeline{ PipelineStorage::Instance->getPipeline(staticTexture2DBatch.getUniquePipelineID()) };
		batchPipeline.bind();

		const auto& transformsSSBO{ ShaderBufferStorage::Instance->getSSBO(staticTexture2DBatch.getTransformsSSBOindex()) };
		transformsSSBO.bind();

		const auto& pointLightSSBO{ ShaderBufferStorage::Instance->getSSBO(pointLightBatch.getPointLightSSBOindex()) };
		pointLightSSBO.bind();

		{ // pass textures 2d
			const FTextureArray& textures{ staticTexture2DBatch.getTextures() };
			std::for_each(textures.cbegin(), textures.cend(), [this](const TexturePair& texturePair) {
				const int32_t textureID{ (int32_t)TextureOpenGL::Instance()->loadTexture(texturePair.texturePath) };

				TextureOpenGL::Instance()->bind2D(texturePair.bindingIndex, textureID);
				m_shaderTexture2D.setUniformSampler(GLSLShaderInfo::samplerTexture2DArray[texturePair.bindingIndex], texturePair.bindingIndex);
			});
		}

		DrawingOpenGL::drawTriangles(staticTexture2DBatch.getIndices().size());
		FRendererEvents::onDrawCall();
	}


}
