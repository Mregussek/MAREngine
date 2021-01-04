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
			const std::vector<UniformItem> cameraItems{ GLSL_SSBOs::ut_u_MVP };

			auto& camera = ShaderBufferStorage::Instance->createShaderBufferStorage();
			RenderMemorizer::Instance->cameraSSBO = ShaderBufferStorage::Instance->getSSBOs().size() - 1;
			camera.initialize(GLSL_SSBOs::ub_Camera, cameraItems);
		}

		{ // setup EntityCmp SSBO
			auto& entityCmp = ShaderBufferStorage::Instance->createShaderBufferStorage();
			std::vector<UniformItem> entitycmpItems{ 
				GLSL_SSBOs::ut_u_SeparateTransform
			};

			entityCmp.initialize(GLSL_SSBOs::ub_EntityCmp, std::move(entitycmpItems));
		}

		{ // setup Material SSBO
			auto& material = ShaderBufferStorage::Instance->createShaderBufferStorage();
			std::vector<UniformItem> materialItems{ 
				GLSL_SSBOs::ut_u_material, GLSL_SSBOs::ut_u_lightSize
			};

			material.initialize(GLSL_SSBOs::ub_Material, std::move(materialItems));
		}

		{ // setup TextureSmaplers SSBO
			auto& textureSamplers = ShaderBufferStorage::Instance->createShaderBufferStorage();
			std::vector<UniformItem> textureSamplersItems{ GLSL_SSBOs::ut_u_Color };

			textureSamplers.initialize(GLSL_SSBOs::ub_TextureSamplers, std::move(textureSamplersItems));
		}
	}

	void RendererBatch::setupShaders() {
		GRAPHICS_TRACE("RENDERER_BATCH: going to setup shaders...");

		const char* vert = "resources/shaders/batcher.vert.glsl";
		{
			const char* frag = "resources/shaders/color.frag.glsl";
			const ShaderPaths shaderPaths(vert, frag, nullptr);

			m_shaderColors.initialize(shaderPaths);
			m_shaderColors.setupShaderUniforms(GLSL_SSBOs::u_2D);
		}
		{
			const char* frag = "resources/shaders/texture2d.frag.glsl";
			const ShaderPaths shaderPaths(vert, frag, nullptr);

			m_shader2D.initialize(shaderPaths);
			m_shader2D.setupShaderUniforms(GLSL_SSBOs::u_2D);
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

		const auto& lights = renderPipeline.getLightContainers();

		const auto& cameraSSBO{ ShaderBufferStorage::Instance->getSSBO(RenderMemorizer::Instance->cameraSSBO) };
		cameraSSBO.bind();

		m_shaderColors.bind();
		drawWithShader(m_shaderColors, lights, renderPipeline.getColorContainers());
		m_shader2D.bind();
		drawWithShader(m_shader2D, lights, renderPipeline.get2Dcontainers());
		//drawWithShader(m_shader2D, lights, renderPipeline.getCubemapContainers());

		GRAPHICS_INFO("RENDERER_BATCH: drawn data given from render pipeline!");
	}

	void RendererBatch::drawWithShader(const ShaderOpenGL& shader, const std::vector<LightContainer>& lights, const std::vector<RenderContainer>& containers) const {
		auto pushContainerToLight = [&shader, &lights, this](const RenderContainer& container) {
			passTransformsToSSBO(container);
			passColorsToSSBO(container.getColors());

			if(container.doesContain2Dtextures()) { passTexturesToShader(shader, container.getTexture2D()); }
			else { passCubemapsToShader(shader, container.getTextureCubemap()); }
			
			const auto& containerBuffer{ PipelineStorage::Instance->getPipeline(container.getUniqueContainerID()) };
			containerBuffer.bind();

			for (const auto& light : lights) {
				passLightToSSBO(light.getLightMaterials());

				DrawingOpenGL::drawTriangles(container.getIndices().size());

				RenderEvents::Instance().onDrawCall();
			}
		};

		std::for_each(containers.cbegin(), containers.cend(), pushContainerToLight);
	}

	void RendererBatch::passTransformsToSSBO(const RenderContainer& container) const {
		const auto& transforms = container.getTransforms();
		const auto& samplerTypes = container.getSamplerTypes();

		const auto& entityShaderBuffer = ShaderBufferStorage::Instance->getCorrectShaderBuffer(GLSL_SSBOs::ub_EntityCmp);

		entityShaderBuffer.bind();
		entityShaderBuffer.update<float>(GLSL_SSBOs::ut_u_SeparateTransform.offset, transforms.size() * sizeof(maths::mat4), maths::mat4::value_ptr(transforms));
		entityShaderBuffer.update<float>(GLSL_SSBOs::ut_u_samplerTypes.offset, samplerTypes.size() * sizeof(float), samplerTypes);
	}

	void RendererBatch::passColorsToSSBO(const ColorVector& colors) const {
		const auto& textureShaderBuffer = ShaderBufferStorage::Instance->getCorrectShaderBuffer(GLSL_SSBOs::ub_TextureSamplers);
		textureShaderBuffer.bind();

		std::for_each(colors.cbegin(), colors.cend(), [&textureShaderBuffer](const ColorPair& color) {
			textureShaderBuffer.update<float>(GLSL_SSBOs::ut_u_Color.offset + color.first * sizeof(maths::vec4), sizeof(maths::vec4), &color.second.x);
		});

		GRAPHICS_INFO("RENDERER_BATCH: passed colors to shader");
	}

	void RendererBatch::passTexturesToShader(const ShaderOpenGL& shader, const TextureVector& textures) const {
		GRAPHICS_INFO("RENDERER_BATCH: passing textures data to shader!");

		std::for_each(textures.cbegin(), textures.cend(), [&shader](const TexturePair& texture) {
			const auto textureID = (int32_t)TextureOpenGL::Instance()->loadTexture(texture.second);
			const auto samplerIndex = (uint32_t)texture.first;

			TextureOpenGL::Instance()->bind2D(samplerIndex, textureID);
			shader.setUniformSampler(GLSL_SSBOs::u_2D[samplerIndex], samplerIndex);
		});

		GRAPHICS_INFO("RENDERER_BATCH: passed textures 2d to shader");
	}

	void RendererBatch::passCubemapsToShader(const ShaderOpenGL& shader, const TextureVector& cubemaps) const {
		std::for_each(cubemaps.cbegin(), cubemaps.cend(), [&shader](const TexturePair& texture) {
			const auto textureID = (int32_t)TextureOpenGL::Instance()->loadCubemap(texture.second);
			const auto samplerIndex = (uint32_t)texture.first;

			TextureOpenGL::Instance()->bindCube(samplerIndex, textureID);
			shader.setUniformSampler(GLSL_SSBOs::u_Cubemap[samplerIndex], samplerIndex);
		});

		GRAPHICS_INFO("RENDERER_BATCH: passed texture cubemaps to shader");
	}

	void RendererBatch::passLightToSSBO(const std::vector<LightMaterial>& lightMaterials) const {
		GRAPHICS_INFO("RENDERER_BATCH: passing light data to shader!");

		const auto lightSize = (int32_t)lightMaterials.size();
		const auto& lightShaderBuffer = ShaderBufferStorage::Instance->getCorrectShaderBuffer(GLSL_SSBOs::ub_Material);

		lightShaderBuffer.bind();

		lightShaderBuffer.update<float>(GLSL_SSBOs::ut_u_material.offset, sizeof(LightMaterial) * lightMaterials.size(), &lightMaterials[0].position.x);
		lightShaderBuffer.update<int32_t>(GLSL_SSBOs::ut_u_lightSize.offset, sizeof(int32_t), &lightSize);

		GRAPHICS_INFO("RENDERER_BATCH: passed light to shader!");
	}


}
