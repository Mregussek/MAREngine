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
#include "RenderCamera.h"
#include "RenderEvents.h"
#include "../GraphicsLogs.h"
#include "../../../Platform/OpenGL/DrawingOpenGL.h"
#include "../../../Platform/OpenGL/ShaderUniforms.h"
#include "../../../Platform/OpenGL/TextureOpenGL.h"


namespace mar::graphics {


	void RendererBatch::initialize() {
		GRAPHICS_INFO("RENDERER_BATCH: going to initialize!");

		const uint32_t size_vertices = settings::maxVerticesCount * sizeof(Vertex);
		const uint32_t size_indices = settings::maxIndicesCount * sizeof(uint32_t);

		m_buffers.initialize(size_vertices, size_indices);

		const char* vert = "resources/shaders/batcher.vert.glsl";
		const char* frag = "resources/shaders/texture2d.frag.glsl";
		const auto shaderPaths = platforms::ShaderPaths(vert, frag, nullptr);

		m_shader.initialize(shaderPaths);

		setupSSBOs();
		setupUBOs();
		
		GRAPHICS_INFO("RENDERER_BATCH: initialized!");
	}

	void RendererBatch::setupSSBOs() {
		using namespace platforms::ShaderUniforms;

		{ // setup Camera SSBO
			auto& camera = m_shader.createShaderBufferStorage();
			std::vector<UniformItem> cameraItems{ ut_u_Model, ut_u_MVP, ut_u_CameraPos };

			camera.initialize(ub_Camera, std::move(cameraItems));
		}

		{ // setup EntityCmp SSBO
			auto& entityCmp = m_shader.createShaderBufferStorage();
			std::vector<UniformItem> entitycmpItems{ ut_u_SeparateTransform, ut_u_samplerTypes };

			entityCmp.initialize(ub_EntityCmp, std::move(entitycmpItems));
		}

		{ // setup Material SSBO
			auto& material = m_shader.createShaderBufferStorage();
			std::vector<UniformItem> materialItems{ ut_u_material, ut_u_lightSize };

			material.initialize(ub_Material, std::move(materialItems));
		}

		{ // setup TextureSmaplers SSBO
			auto& textureSamplers = m_shader.createShaderBufferStorage();
			std::vector<UniformItem> textureSamplersItems{ ut_u_Color };

			textureSamplers.initialize(ub_TextureSamplers, std::move(textureSamplersItems));
		}
	}

	void RendererBatch::setupUBOs() {
		using namespace platforms::ShaderUniforms;

	}

	void RendererBatch::close() {
		GRAPHICS_INFO("RENDERER_BATCH: going to close!");

		m_buffers.close();
		m_shader.shutdown();
		platforms::TextureOpenGL::Instance()->shutdown();

		GRAPHICS_INFO("RENDERER_BATCH: closed!");
	}

	void RendererBatch::draw(RenderPipeline& renderPipeline) {
		GRAPHICS_TRACE("RENDERER_BATCH: going to draw render pipeline!");
		
		const auto& camera = *renderPipeline.getCamera();
		const auto& containers2D = renderPipeline.get2Dcontainers();
		const auto& containersCubemap = renderPipeline.getCubemapContainers();
		const auto& lights = renderPipeline.getLightContainers();

		auto pushContainerToLight = [&lights, &camera, this](const RenderContainer& container) {
			std::for_each(lights.cbegin(), lights.cend(), [&container, &camera, this](const LightContainer& light) {
				m_shader.bind();

				passCameraToShader(m_shader, camera);
				passLightToShader(m_shader, light.getLightMaterials());
				passTransformsToShader(m_shader, container);
				passColorsToShader(m_shader, container);
				passTexturesToShader(m_shader, container);

				drawContainer(container);

				RenderEvents::Instance().onDrawCall();
			});
		};

		std::for_each(containers2D.cbegin(), containers2D.cend(), pushContainerToLight);
		std::for_each(containersCubemap.cbegin(), containersCubemap.cend(), pushContainerToLight);

		GRAPHICS_INFO("RENDERER_BATCH: drawn data given from render pipeline!");
	}

	void RendererBatch::drawContainer(const RenderContainer& container) const {
		GRAPHICS_TRACE("RENDERER_BATCH: going to draw render container!");
		
		{
			m_buffers.bind();
			m_buffers.update(container.getVertices(), container.getIndices());

			platforms::DrawingOpenGL::drawTriangles(container.getIndices().size());

			m_buffers.reset();
			m_buffers.unbind();

			platforms::TextureOpenGL::Instance()->unbind(container.getSamplerTypes());
		}

		GRAPHICS_INFO("RENDERER_BATCH: drawn data given from render container!");
	}

	void RendererBatch::passTransformsToShader(const platforms::ShaderOpenGL& shader, const RenderContainer& container) const {
		using namespace platforms::ShaderUniforms;

		const auto& transforms = container.getTransforms();
		const auto& samplerTypes = container.getSamplerTypes();

		const auto& entityShaderBuffer = shader.getCorrectShaderBuffer(ub_EntityCmp);

		entityShaderBuffer.bind();
		entityShaderBuffer.update<float>(ut_u_SeparateTransform.offset, transforms.size() * sizeof(maths::mat4), maths::mat4::value_ptr(transforms));
		entityShaderBuffer.update<float>(ut_u_samplerTypes.offset, samplerTypes.size() * sizeof(float), samplerTypes);
	}

	void RendererBatch::passColorsToShader(const platforms::ShaderOpenGL& shader, const RenderContainer& container) const {
		using namespace platforms::ShaderUniforms;
		using namespace mar::maths;

		auto passColors = [&shader, &colors = container.getColors()]() {
			const auto& textureShaderBuffer = shader.getCorrectShaderBuffer(ub_TextureSamplers);
			textureShaderBuffer.bind();

			std::for_each(colors.cbegin(), colors.cend(), [&textureShaderBuffer](const ColorPair& color) {
				textureShaderBuffer.update<float>(ut_u_Color.offset + color.first * sizeof(vec4), sizeof(vec4), &color.second.x);
			});

			GRAPHICS_INFO("RENDERER_BATCH: passed colors to shader");
		};

		passColors();
	}

	void RendererBatch::passTexturesToShader(const platforms::ShaderOpenGL& shader, const RenderContainer& container) const {
		GRAPHICS_INFO("RENDERER_BATCH: passing textures data to shader!");

		using namespace platforms::ShaderUniforms;
		typedef platforms::TextureOpenGL TextureGL;

		const auto& textures = container.getTexture2D();
		
		auto pass2D = [&shader, &textures]() {
			std::for_each(textures.cbegin(), textures.cend(), [&shader](const TexturePair& texture) {
				const auto textureID = (int32_t)TextureGL::Instance()->loadTexture(texture.second);
				const auto samplerIndex = (uint32_t)texture.first;

				TextureGL::Instance()->bind2D(samplerIndex, textureID);
				shader.setUniformSampler(u_2D[samplerIndex], samplerIndex);
			});

			GRAPHICS_INFO("RENDERER_BATCH: passed textures 2d to shader");
		};

		pass2D();
	}

	void RendererBatch::passCubemapsToShader(const platforms::ShaderOpenGL& shader, const RenderContainer& container) const {
		using namespace platforms::ShaderUniforms;
		typedef platforms::TextureOpenGL TextureGL;

		const auto& cubemaps = container.getTextureCubemap();

		auto passCubemap = [&shader, &cubemaps]() {
			std::for_each(cubemaps.cbegin(), cubemaps.cend(), [&shader](const TexturePair& texture) {
				const auto textureID = (int32_t)TextureGL::Instance()->loadCubemap(texture.second);
				const auto samplerIndex = (uint32_t)texture.first;

				TextureGL::Instance()->bindCube(samplerIndex, textureID);
				shader.setUniformSampler(u_Cubemap[samplerIndex], samplerIndex);
			});

			GRAPHICS_INFO("RENDERER_BATCH: passed texture cubemaps to shader");
		};

		passCubemap();
	}

	void RendererBatch::passLightToShader(const platforms::ShaderOpenGL& shader, const std::vector<LightMaterial>& lightMaterials) const {
		GRAPHICS_INFO("RENDERER_BATCH: passing light data to shader!");

		using namespace platforms::ShaderUniforms;

		const auto lightSize = (int32_t)lightMaterials.size();

		const auto& lightShaderBuffer = shader.getCorrectShaderBuffer(ub_Material);

		lightShaderBuffer.bind();

		lightShaderBuffer.update<float>(ut_u_material.offset, sizeof(graphics::LightMaterial) * lightMaterials.size(), &lightMaterials[0].position.x);
		lightShaderBuffer.update<int32_t>(ut_u_lightSize.offset, sizeof(int32_t), &lightSize);

		GRAPHICS_INFO("RENDERER_BATCH: passed light to shader!");
	}

	void RendererBatch::passCameraToShader(const platforms::ShaderOpenGL& shader, const RenderCamera& camera) const {
		GRAPHICS_INFO("RENDERER_BATCH: passing camera data to shader!");

		using namespace platforms::ShaderUniforms;
		using namespace mar::maths;

		const auto& cameraShaderBuffer = shader.getCorrectShaderBuffer(ub_Camera);

		cameraShaderBuffer.bind();

		cameraShaderBuffer.update<float>(ut_u_CameraPos.offset, sizeof(vec3), vec3::value_ptr(camera.getPosition()));
		cameraShaderBuffer.update<float>(ut_u_Model.offset, sizeof(mat4), mat4::value_ptr(camera.getModel()));
		cameraShaderBuffer.update<float>(ut_u_MVP.offset, sizeof(mat4), mat4::value_ptr(camera.getMVP()));

		GRAPHICS_INFO("RENDERER_BATCH: passed camera to shader!");
	}


}
