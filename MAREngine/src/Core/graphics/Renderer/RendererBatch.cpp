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
#include "RenderPipeline.h"
#include "RenderEvents.h"
#include "ShaderBufferStorage.h"
#include "RenderMemorizer.h"
#include "../GraphicsLogs.h"
#include "../GraphicLimits.h"
#include "../../../Platform/OpenGL/DrawingOpenGL.h"
#include "../../../Platform/OpenGL/ShaderUniforms.h"
#include "../../../Platform/OpenGL/TextureOpenGL.h"


namespace mar::graphics {


	void RendererBatch::initialize() {
		GRAPHICS_INFO("RENDERER_BATCH: going to initialize!");

		using namespace platforms::ShaderUniforms;

		m_buffers.initialize(GraphicLimits::sizeOfVertices, GraphicLimits::sizeOfIndices);

		setupSSBOs();

		const char* vert = "resources/shaders/batcher.vert.glsl";

		{
			const char* frag = "resources/shaders/texture2d.frag.glsl";
			const auto shaderPaths = platforms::ShaderPaths(vert, frag, nullptr);

			m_shader2D.initialize(shaderPaths);
			m_shader2D.setupShaderUniforms(u_2D);
		}
		{
			const char* frag = "resources/shaders/cubemap.frag.glsl";
			const auto shaderPaths = platforms::ShaderPaths(vert, frag, nullptr);

			m_shaderCubemap.initialize(shaderPaths);
			m_shaderCubemap.setupShaderUniforms(u_Cubemap);
		}
		
		GRAPHICS_INFO("RENDERER_BATCH: initialized!");
	}

	void RendererBatch::setupSSBOs() {
		using namespace platforms;
		using namespace platforms::ShaderUniforms;

		{ // setup Camera SSBO
			const std::vector<UniformItem> cameraItems{ ut_u_Model, ut_u_MVP, ut_u_CameraPos };

			auto& camera = ShaderBufferStorage::Instance->createShaderBufferStorage();
			RenderMemorizer::Instance->cameraSSBO = ShaderBufferStorage::Instance->getSSBOs().size() - 1;
			camera.initialize(ub_Camera, cameraItems);
		}

		{ // setup EntityCmp SSBO
			auto& entityCmp = ShaderBufferStorage::Instance->createShaderBufferStorage();
			std::vector<UniformItem> entitycmpItems{ ut_u_SeparateTransform, ut_u_samplerTypes };

			entityCmp.initialize(ub_EntityCmp, std::move(entitycmpItems));
		}

		{ // setup Material SSBO
			auto& material = ShaderBufferStorage::Instance->createShaderBufferStorage();
			std::vector<UniformItem> materialItems{ ut_u_material, ut_u_lightSize };

			material.initialize(ub_Material, std::move(materialItems));
		}

		{ // setup TextureSmaplers SSBO
			auto& textureSamplers = ShaderBufferStorage::Instance->createShaderBufferStorage();
			std::vector<UniformItem> textureSamplersItems{ ut_u_Color };

			textureSamplers.initialize(ub_TextureSamplers, std::move(textureSamplersItems));
		}
	}

	void RendererBatch::close() {
		GRAPHICS_INFO("RENDERER_BATCH: going to close!");

		ShaderBufferStorage::Instance->close();
		m_buffers.close();
		m_shader2D.shutdown();
		m_shaderCubemap.shutdown();
		platforms::TextureOpenGL::Instance()->shutdown();

		GRAPHICS_INFO("RENDERER_BATCH: closed!");
	}

	void RendererBatch::draw(const RenderPipeline& renderPipeline) {
		GRAPHICS_TRACE("RENDERER_BATCH: going to draw render pipeline!");

		const auto& lights = renderPipeline.getLightContainers();

		const auto& cameraSSBO{ ShaderBufferStorage::Instance->getSSBO(RenderMemorizer::Instance->cameraSSBO) };
		cameraSSBO.bind();

		m_shader2D.bind();
		drawWithShader(m_shader2D, lights, renderPipeline.get2Dcontainers());

		m_shaderCubemap.bind();
		drawWithShader(m_shaderCubemap, lights, renderPipeline.getCubemapContainers());

		GRAPHICS_INFO("RENDERER_BATCH: drawn data given from render pipeline!");
	}

	void RendererBatch::drawWithShader(const platforms::ShaderOpenGL& shader, const std::vector<LightContainer>& lights, const std::vector<RenderContainer>& containers) const {
		auto pushContainerToLight = [&shader, &lights, this](const RenderContainer& container) {
			passTransformsToSSBO(container);
			passColorsToSSBO(container.getColors());

			if(container.doesContain2Dtextures()) { passTexturesToShader(shader, container.getTexture2D()); }
			else { passCubemapsToShader(shader, container.getTextureCubemap()); }
			
			passVerticesAndIndicesToBuffer(container.getVertices(), container.getIndices());

			for (const auto& light : lights) {
				passLightToSSBO(light.getLightMaterials());

				platforms::DrawingOpenGL::drawTriangles(container.getIndices().size());

				RenderEvents::Instance().onDrawCall();
			}
		};

		std::for_each(containers.cbegin(), containers.cend(), pushContainerToLight);
	}

	void RendererBatch::passVerticesAndIndicesToBuffer(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) const {
		m_buffers.bind();
		m_buffers.update(vertices, indices);
	}

	void RendererBatch::passTransformsToSSBO(const RenderContainer& container) const {
		using namespace platforms::ShaderUniforms;

		const auto& transforms = container.getTransforms();
		const auto& samplerTypes = container.getSamplerTypes();

		const auto& entityShaderBuffer = ShaderBufferStorage::Instance->getCorrectShaderBuffer(ub_EntityCmp);

		entityShaderBuffer.bind();
		entityShaderBuffer.update<float>(ut_u_SeparateTransform.offset, transforms.size() * sizeof(maths::mat4), maths::mat4::value_ptr(transforms));
		entityShaderBuffer.update<float>(ut_u_samplerTypes.offset, samplerTypes.size() * sizeof(float), samplerTypes);
	}

	void RendererBatch::passColorsToSSBO(const ColorVector& colors) const {
		using namespace platforms::ShaderUniforms;
		using namespace mar::maths;

		const auto& textureShaderBuffer = ShaderBufferStorage::Instance->getCorrectShaderBuffer(ub_TextureSamplers);
		textureShaderBuffer.bind();

		std::for_each(colors.cbegin(), colors.cend(), [&textureShaderBuffer](const ColorPair& color) {
			textureShaderBuffer.update<float>(ut_u_Color.offset + color.first * sizeof(vec4), sizeof(vec4), &color.second.x);
		});

		GRAPHICS_INFO("RENDERER_BATCH: passed colors to shader");
	}

	void RendererBatch::passTexturesToShader(const platforms::ShaderOpenGL& shader, const TextureVector& textures) const {
		GRAPHICS_INFO("RENDERER_BATCH: passing textures data to shader!");

		using namespace platforms::ShaderUniforms;
		typedef platforms::TextureOpenGL TextureGL;

		std::for_each(textures.cbegin(), textures.cend(), [&shader](const TexturePair& texture) {
			const auto textureID = (int32_t)TextureGL::Instance()->loadTexture(texture.second);
			const auto samplerIndex = (uint32_t)texture.first;

			TextureGL::Instance()->bind2D(samplerIndex, textureID);
			shader.setUniformSampler(u_2D[samplerIndex], samplerIndex);
		});

		GRAPHICS_INFO("RENDERER_BATCH: passed textures 2d to shader");
	}

	void RendererBatch::passCubemapsToShader(const platforms::ShaderOpenGL& shader, const TextureVector& cubemaps) const {
		using namespace platforms::ShaderUniforms;
		typedef platforms::TextureOpenGL TextureGL;

		std::for_each(cubemaps.cbegin(), cubemaps.cend(), [&shader](const TexturePair& texture) {
			const auto textureID = (int32_t)TextureGL::Instance()->loadCubemap(texture.second);
			const auto samplerIndex = (uint32_t)texture.first;

			TextureGL::Instance()->bindCube(samplerIndex, textureID);
			shader.setUniformSampler(u_Cubemap[samplerIndex], samplerIndex);
		});

		GRAPHICS_INFO("RENDERER_BATCH: passed texture cubemaps to shader");
	}

	void RendererBatch::passLightToSSBO(const std::vector<LightMaterial>& lightMaterials) const {
		GRAPHICS_INFO("RENDERER_BATCH: passing light data to shader!");

		using namespace platforms::ShaderUniforms;

		const auto lightSize = (int32_t)lightMaterials.size();
		const auto& lightShaderBuffer = ShaderBufferStorage::Instance->getCorrectShaderBuffer(ub_Material);

		lightShaderBuffer.bind();

		lightShaderBuffer.update<float>(ut_u_material.offset, sizeof(graphics::LightMaterial) * lightMaterials.size(), &lightMaterials[0].position.x);
		lightShaderBuffer.update<int32_t>(ut_u_lightSize.offset, sizeof(int32_t), &lightSize);

		GRAPHICS_INFO("RENDERER_BATCH: passed light to shader!");
	}


}
