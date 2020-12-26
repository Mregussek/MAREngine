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


namespace mar::graphics {


	void RendererBatch::initialize() {
		GRAPHICS_INFO("RENDERER_BATCH: going to initialize!");

		setupSSBOs();
		setupShaders();
	}

	void RendererBatch::setupSSBOs() {
		GRAPHICS_TRACE("RENDERER_BATCH: going to setup shader storage buffers...");

		{ // setup Camera SSBO
			const std::vector<platforms::UniformItem> cameraItems{ SSBOsGL::ut_u_MVP };

			auto& camera = ShaderBufferStorage::Instance->createShaderBufferStorage();
			RenderMemorizer::Instance->cameraSSBO = ShaderBufferStorage::Instance->getSSBOs().size() - 1;
			camera.initialize(SSBOsGL::ub_Camera, cameraItems);
		}

		{ // setup EntityCmp SSBO
			auto& entityCmp = ShaderBufferStorage::Instance->createShaderBufferStorage();
			std::vector<platforms::UniformItem> entitycmpItems{ 
				SSBOsGL::ut_u_SeparateTransform, SSBOsGL::ut_u_samplerTypes
			};

			entityCmp.initialize(SSBOsGL::ub_EntityCmp, std::move(entitycmpItems));
		}

		{ // setup Material SSBO
			auto& material = ShaderBufferStorage::Instance->createShaderBufferStorage();
			std::vector<platforms::UniformItem> materialItems{ 
				SSBOsGL::ut_u_material, SSBOsGL::ut_u_lightSize 
			};

			material.initialize(SSBOsGL::ub_Material, std::move(materialItems));
		}

		{ // setup TextureSmaplers SSBO
			auto& textureSamplers = ShaderBufferStorage::Instance->createShaderBufferStorage();
			std::vector<platforms::UniformItem> textureSamplersItems{ SSBOsGL::ut_u_Color };

			textureSamplers.initialize(SSBOsGL::ub_TextureSamplers, std::move(textureSamplersItems));
		}
	}

	void RendererBatch::setupShaders() {
		GRAPHICS_TRACE("RENDERER_BATCH: going to setup shaders...");

		const char* vert = "resources/shaders/batcher.vert.glsl";
		{
			const char* frag = "resources/shaders/texture2d.frag.glsl";
			const platforms::ShaderPaths shaderPaths(vert, frag, nullptr);

			m_shader2D.initialize(shaderPaths);
			m_shader2D.setupShaderUniforms(SSBOsGL::u_2D);
		}
	}

	void RendererBatch::close() {
		GRAPHICS_INFO("RENDERER_BATCH: going to close!");

		m_shader2D.shutdown();
		platforms::TextureOpenGL::Instance()->shutdown();

		GRAPHICS_INFO("RENDERER_BATCH: closed!");
	}

	void RendererBatch::draw(const RenderPipeline& renderPipeline) {
		GRAPHICS_TRACE("RENDERER_BATCH: going to draw render pipeline!");

		const auto& lights = renderPipeline.getLightContainers();

		const auto& cameraSSBO{ ShaderBufferStorage::Instance->getSSBO(RenderMemorizer::Instance->cameraSSBO) };
		cameraSSBO.bind();

		m_shader2D.bind();
		drawWithShader(m_shader2D, lights, renderPipeline.getColorContainers());
		drawWithShader(m_shader2D, lights, renderPipeline.get2Dcontainers());
		drawWithShader(m_shader2D, lights, renderPipeline.getCubemapContainers());

		GRAPHICS_INFO("RENDERER_BATCH: drawn data given from render pipeline!");
	}

	void RendererBatch::drawWithShader(const ShaderGL& shader, const std::vector<LightContainer>& lights, const std::vector<RenderContainer>& containers) const {
		auto pushContainerToLight = [&shader, &lights, this](const RenderContainer& container) {
			passTransformsToSSBO(container);
			passColorsToSSBO(container.getColors());

			if(container.doesContain2Dtextures()) { passTexturesToShader(shader, container.getTexture2D()); }
			else { passCubemapsToShader(shader, container.getTextureCubemap()); }
			
			const auto& containerBuffer{ PipelineStorage::Instance->getPipeline(container.getUniqueContainerID()) };
			containerBuffer.bind();

			for (const auto& light : lights) {
				passLightToSSBO(light.getLightMaterials());

				platforms::DrawingOpenGL::drawTriangles(container.getIndices().size());

				RenderEvents::Instance().onDrawCall();
			}
		};

		std::for_each(containers.cbegin(), containers.cend(), pushContainerToLight);
	}

	void RendererBatch::passTransformsToSSBO(const RenderContainer& container) const {
		const auto& transforms = container.getTransforms();
		const auto& samplerTypes = container.getSamplerTypes();

		const auto& entityShaderBuffer = ShaderBufferStorage::Instance->getCorrectShaderBuffer(SSBOsGL::ub_EntityCmp);

		entityShaderBuffer.bind();
		entityShaderBuffer.update<float>(SSBOsGL::ut_u_SeparateTransform.offset, transforms.size() * sizeof(maths::mat4), maths::mat4::value_ptr(transforms));
		entityShaderBuffer.update<float>(SSBOsGL::ut_u_samplerTypes.offset, samplerTypes.size() * sizeof(float), samplerTypes);
	}

	void RendererBatch::passColorsToSSBO(const ColorVector& colors) const {
		using namespace mar::maths;

		const auto& textureShaderBuffer = ShaderBufferStorage::Instance->getCorrectShaderBuffer(SSBOsGL::ub_TextureSamplers);
		textureShaderBuffer.bind();

		std::for_each(colors.cbegin(), colors.cend(), [&textureShaderBuffer](const ColorPair& color) {
			textureShaderBuffer.update<float>(SSBOsGL::ut_u_Color.offset + color.first * sizeof(vec4), sizeof(vec4), &color.second.x);
		});

		GRAPHICS_INFO("RENDERER_BATCH: passed colors to shader");
	}

	void RendererBatch::passTexturesToShader(const ShaderGL& shader, const TextureVector& textures) const {
		GRAPHICS_INFO("RENDERER_BATCH: passing textures data to shader!");

		std::for_each(textures.cbegin(), textures.cend(), [&shader](const TexturePair& texture) {
			const auto textureID = (int32_t)TextureGL::Instance()->loadTexture(texture.second);
			const auto samplerIndex = (uint32_t)texture.first;

			TextureGL::Instance()->bind2D(samplerIndex, textureID);
			shader.setUniformSampler(SSBOsGL::u_2D[samplerIndex], samplerIndex);
		});

		GRAPHICS_INFO("RENDERER_BATCH: passed textures 2d to shader");
	}

	void RendererBatch::passCubemapsToShader(const ShaderGL& shader, const TextureVector& cubemaps) const {
		std::for_each(cubemaps.cbegin(), cubemaps.cend(), [&shader](const TexturePair& texture) {
			const auto textureID = (int32_t)TextureGL::Instance()->loadCubemap(texture.second);
			const auto samplerIndex = (uint32_t)texture.first;

			TextureGL::Instance()->bindCube(samplerIndex, textureID);
			shader.setUniformSampler(SSBOsGL::u_Cubemap[samplerIndex], samplerIndex);
		});

		GRAPHICS_INFO("RENDERER_BATCH: passed texture cubemaps to shader");
	}

	void RendererBatch::passLightToSSBO(const std::vector<LightMaterial>& lightMaterials) const {
		GRAPHICS_INFO("RENDERER_BATCH: passing light data to shader!");

		const auto lightSize = (int32_t)lightMaterials.size();
		const auto& lightShaderBuffer = ShaderBufferStorage::Instance->getCorrectShaderBuffer(SSBOsGL::ub_Material);

		lightShaderBuffer.bind();

		lightShaderBuffer.update<float>(SSBOsGL::ut_u_material.offset, sizeof(graphics::LightMaterial) * lightMaterials.size(), &lightMaterials[0].position.x);
		lightShaderBuffer.update<int32_t>(SSBOsGL::ut_u_lightSize.offset, sizeof(int32_t), &lightSize);

		GRAPHICS_INFO("RENDERER_BATCH: passed light to shader!");
	}


}
