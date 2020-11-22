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

		m_buffers.initialize(settings::maxVerticesCount * sizeof(Vertex), settings::maxIndicesCount * sizeof(uint32_t));

		const char* vert = "resources/shaders/batcher.vert.glsl";
		const char* frag = "resources/shaders/batcher.frag.glsl";
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

	void RendererBatch::draw(RenderPipeline& render_pip) {
		GRAPHICS_TRACE("RENDERER_BATCH: going to draw render pipeline!");
		
		for (const auto& container : render_pip.getContainers()) {
			for (const auto& container_light : render_pip.getContainers()) {
				if (!container_light.getLights().empty()) {
					m_shader.bind();

					passCameraToShader(render_pip.getCamera());
					passLightToShader(container_light);

					drawContainer(container);

					RenderEvents::Instance().onDrawCall();
				}
			}
		}
		
		GRAPHICS_INFO("RENDERER_BATCH: drawn data given from render pipeline!");
	}

	void RendererBatch::drawContainer(const RenderContainer& container) const {
		GRAPHICS_TRACE("RENDERER_BATCH: going to draw render container!");
		
		{
			passTransformsToShader(container);
			passTexturesToShader(container);
		}
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

	void RendererBatch::passTransformsToShader(const RenderContainer& container) const {
		using namespace platforms::ShaderUniforms;

		const auto& transforms = container.getTransforms();
		const auto& samplerTypes = container.getSamplerTypes();

		const auto& entityShaderBuffer = m_shader.getCorrectShaderBuffer(ub_EntityCmp);

		entityShaderBuffer.bind();
		entityShaderBuffer.update<float>(ut_u_SeparateTransform.offset, transforms.size() * sizeof(maths::mat4), maths::mat4::value_ptr(transforms));
		entityShaderBuffer.update<float>(ut_u_samplerTypes.offset, samplerTypes.size() * sizeof(float), samplerTypes);
	}

	void RendererBatch::passTexturesToShader(const RenderContainer& container) const {
		GRAPHICS_INFO("RENDERER_BATCH: passing textures data to shader!");

		using namespace platforms::ShaderUniforms;
		using namespace mar::maths;
		typedef platforms::TextureOpenGL TextureGL;

		const auto& colors = container.getColors();
		const auto& textures = container.getTexture2D();
		const auto& cubemaps = container.getTextureCubemap();

		auto passColors = [this, &colors]() {
			const auto& textureShaderBuffer = m_shader.getCorrectShaderBuffer(ub_TextureSamplers);
			textureShaderBuffer.bind();

			std::for_each(colors.begin(), colors.end(), [&textureShaderBuffer](const ColorPair& color) {
				textureShaderBuffer.update<float>(ut_u_Color.offset + color.first * sizeof(vec4), sizeof(vec4), &color.second.x);
			});

			GRAPHICS_INFO("RENDERER_BATCH: passed colors to shader");
		};

		auto pass2D = [this, &textures]() {
			std::for_each(textures.begin(), textures.end(), [&shader = m_shader](const TexturePair& texture) {
				const auto textureID = (int32_t)TextureGL::Instance()->loadTexture(texture.second);
				const auto samplerIndex = (uint32_t)texture.first;

				TextureGL::Instance()->bind2D(samplerIndex, textureID);
				shader.setUniformSampler(u_2D[samplerIndex], samplerIndex);
			});

			GRAPHICS_INFO("RENDERER_BATCH: passed textures 2d to shader");
		};

		auto passCubemap = [this, &cubemaps]() {
			std::for_each(cubemaps.begin(), cubemaps.end(), [&shader = m_shader](const TexturePair& texture) {
				const auto textureID = (int32_t)TextureGL::Instance()->loadCubemap(texture.second);
				const auto samplerIndex = (uint32_t)texture.first;

				TextureGL::Instance()->bindCube(samplerIndex, textureID);
				shader.setUniformSampler(u_Cubemap[samplerIndex], samplerIndex);
			});

			GRAPHICS_INFO("RENDERER_BATCH: passed texture cubemaps to shader");
		};

		passColors();
		pass2D();
		//passCubemap();
	}

	void RendererBatch::passLightToShader(const RenderContainer& container) const {
		GRAPHICS_INFO("RENDERER_BATCH: passing light data to shader!");

		using namespace platforms::ShaderUniforms;

		const auto& lights = container.getLights();
		const auto lightSize = (int32_t)lights.size();

		const auto& lightShaderBuffer = m_shader.getCorrectShaderBuffer(ub_Material);

		lightShaderBuffer.bind();

		lightShaderBuffer.update<float>(ut_u_material.offset, sizeof(graphics::LightMaterial) * lights.size(), &lights[0].position.x);
		lightShaderBuffer.update<int32_t>(ut_u_lightSize.offset, sizeof(int32_t), &lightSize);

		GRAPHICS_INFO("RENDERER_BATCH: passed light to shader!");
	}

	void RendererBatch::passCameraToShader(const RenderCamera* camera) const {
		GRAPHICS_INFO("RENDERER_BATCH: passing camera data to shader!");

		using namespace platforms::ShaderUniforms;
		using namespace mar::maths;

		const auto& cameraShaderBuffer = m_shader.getCorrectShaderBuffer(ub_Camera);

		cameraShaderBuffer.bind();

		cameraShaderBuffer.update<float>(ut_u_CameraPos.offset, sizeof(vec3), vec3::value_ptr(camera->getPosition()));
		cameraShaderBuffer.update<float>(ut_u_Model.offset, sizeof(mat4), mat4::value_ptr(camera->getModel()));
		cameraShaderBuffer.update<float>(ut_u_MVP.offset, sizeof(mat4), mat4::value_ptr(camera->getMVP()));

		GRAPHICS_INFO("RENDERER_BATCH: passed camera to shader!");
	}


}
