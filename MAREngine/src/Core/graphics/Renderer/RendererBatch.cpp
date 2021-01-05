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
#include "ShaderBufferStorage.h"
#include "RenderMemorizer.h"
#include "PipelineStorage.h"
#include "../GraphicsLogs.h"
#include "../GraphicLimits.h"
#include "../RenderAPI/RenderPipeline.h"
#include "../Mesh/MeshBatchStaticColor.h"
#include "../../events/RenderEvents.h"
#include "../../../Platform/OpenGL/DrawingOpenGL.h"
#include "../../../Platform/OpenGL/TextureOpenGL.h"
#include "../../../Platform/GLSL/ShaderUniforms.h"


namespace marengine {


	void RendererBatch::initialize() {
		GRAPHICS_INFO("RENDERER_BATCH: going to initialize!");

		setupSSBOs();
		setupShaders();
	}

	void RendererBatch::setupSSBOs() {
		GRAPHICS_TRACE("RENDERER_BATCH: going to setup shader storage buffers...");

		{ // setup Camera SSBO
			const std::vector<UniformItem> cameraItems{ GLSLShaderInfo::MVP };
			auto& cameraSSBO = ShaderBufferStorage::Instance->createShaderBufferStorage();
			RenderMemorizer::Instance->cameraSSBO = ShaderBufferStorage::Instance->getSSBOs().size() - 1;
			
			cameraSSBO.initialize(GLSLShaderInfo::CameraSSBO, cameraItems);
		}

		{ // setup EntityCmp SSBO
			const std::vector<UniformItem> transformItems{ GLSLShaderInfo::Transform };
			auto& transformSSBO = ShaderBufferStorage::Instance->createShaderBufferStorage();

			transformSSBO.initialize(GLSLShaderInfo::TransformSSBO, transformItems);
		}

		{ // setup Material SSBO
			const std::vector<UniformItem> pointLightsItems{ GLSLShaderInfo::LightMaterial, GLSLShaderInfo::LightMaterialSize };
			auto& pointLightsSSBO = ShaderBufferStorage::Instance->createShaderBufferStorage();

			pointLightsSSBO.initialize(GLSLShaderInfo::PointLightSSBO, pointLightsItems);
		}

		{ // setup TextureSmaplers SSBO
			const std::vector<UniformItem> colorItems{ GLSLShaderInfo::Colors };
			auto& colorSSBO = ShaderBufferStorage::Instance->createShaderBufferStorage();
			
			colorSSBO.initialize(GLSLShaderInfo::ColorsSSBO, colorItems);
		}
	}

	void RendererBatch::setupShaders() {
		GRAPHICS_TRACE("RENDERER_BATCH: going to setup shaders...");

		const char* vert = "resources/shaders/default.vert.glsl";
		{
			const char* frag = "resources/shaders/color.frag.glsl";
			const ShaderPaths shaderPaths(vert, frag, nullptr);

			m_shaderColors.initialize(shaderPaths);
		}
		{
			const char* frag = "resources/shaders/texture2d.frag.glsl";
			const ShaderPaths shaderPaths(vert, frag, nullptr);

			m_shader2D.initialize(shaderPaths);
			m_shader2D.setupShaderUniforms(GLSLShaderInfo::samplerTexture2DArray);
		}
	}

	void RendererBatch::close() {
		GRAPHICS_INFO("RENDERER_BATCH: going to close!");

		m_shaderColors.shutdown();
		m_shader2D.shutdown();
		TextureOpenGL::Instance()->shutdown();

		GRAPHICS_INFO("RENDERER_BATCH: closed!");
	}

	void RendererBatch::draw(const RenderPipeline& renderPipeline) {
		GRAPHICS_TRACE("RENDERER_BATCH: going to draw render pipeline!");
		{
			const auto& cameraSSBO{ ShaderBufferStorage::Instance->getSSBO(RenderMemorizer::Instance->cameraSSBO) };
			cameraSSBO.bind();
		}
		{
			const auto& lights = renderPipeline.getLightContainers();
			const auto& lightMaterials{ lights[0].getLightMaterials() };
			const auto lightSize = (int32_t)lightMaterials.size();
			const auto& pointLightSSBO = ShaderBufferStorage::Instance->getCorrectShaderBuffer(GLSLShaderInfo::PointLightSSBO);
			pointLightSSBO.bind();
			pointLightSSBO.update<float>(GLSLShaderInfo::LightMaterial.offset, sizeof(LightMaterial) * lightMaterials.size(), &lightMaterials[0].position.x);
			pointLightSSBO.update<int32_t>(GLSLShaderInfo::LightMaterialSize.offset, sizeof(int32_t), &lightSize);
		}
		
		drawColors(renderPipeline.getColorBatches());
		drawTextures2D(renderPipeline.get2Dcontainers());

		GRAPHICS_INFO("RENDERER_BATCH: drawn data given from render pipeline!");
	}

	void RendererBatch::drawColors(const std::vector<FMeshBatchStaticColor>& batches) const {
		m_shaderColors.bind();

		std::for_each(batches.cbegin(), batches.cend(), [](const FMeshBatchStaticColor& batch) {
			{
				const auto& transforms{ batch.getTransforms() };
				const auto& transformSSBO{ ShaderBufferStorage::Instance->getCorrectShaderBuffer(GLSLShaderInfo::TransformSSBO) };
				transformSSBO.bind();
				transformSSBO.update<float>(GLSLShaderInfo::Transform.offset, transforms.size() * sizeof(maths::mat4), maths::mat4::value_ptr(transforms));
			}
			{
				const auto& colors{ batch.getColors() };
				const auto& colorSSBO{ ShaderBufferStorage::Instance->getCorrectShaderBuffer(GLSLShaderInfo::ColorsSSBO) };
				colorSSBO.bind();
				colorSSBO.update<float>(GLSLShaderInfo::Colors.offset, colors.size() * sizeof(maths::vec4), maths::vec4::value_ptr(colors));
			}
			{
				const auto& containerBuffer{ PipelineStorage::Instance->getPipeline(batch.getUniquePipelineID()) };
				containerBuffer.bind();
			}

			DrawingOpenGL::drawTriangles(batch.getIndices().size());
			RenderEvents::Instance().onDrawCall();
		});
	}

	void RendererBatch::drawTextures2D(const std::vector<RenderContainer>& containers) const {
		m_shader2D.bind();

		std::for_each(containers.cbegin(), containers.cend(), [this](const RenderContainer& container) {
			{
				const auto& transforms{ container.getTransforms() };
				const auto& transformSSBO{ ShaderBufferStorage::Instance->getCorrectShaderBuffer(GLSLShaderInfo::TransformSSBO) };
				transformSSBO.bind();
				transformSSBO.update<float>(GLSLShaderInfo::Transform.offset, transforms.size() * sizeof(maths::mat4), maths::mat4::value_ptr(transforms));
			}
			{
				const auto& textures{ container.getTexture2D() };
				std::for_each(textures.cbegin(), textures.cend(), [this](const TexturePair& texture) {
					const auto textureID = (int32_t)TextureOpenGL::Instance()->loadTexture(texture.second);
					const auto samplerIndex = (uint32_t)texture.first;

					TextureOpenGL::Instance()->bind2D(samplerIndex, textureID);
					m_shader2D.setUniformSampler(GLSLShaderInfo::samplerTexture2DArray[samplerIndex], samplerIndex);
				});
			}
			{
				const auto& containerBuffer{ PipelineStorage::Instance->getPipeline(container.getUniqueContainerID()) };
				containerBuffer.bind();
			}

			DrawingOpenGL::drawTriangles(container.getIndices().size());
			RenderEvents::Instance().onDrawCall();
		});
	}


}
