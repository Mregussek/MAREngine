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

		using namespace platforms::ShaderUniforms;

		m_buffers.initialize(settings::maxVerticesCount * sizeof(Vertex), settings::maxIndicesCount * sizeof(uint32_t));

		const char* vert = "resources/shaders/batcher.vert.glsl";
		const char* frag = "resources/shaders/batcher.frag.glsl";
		const auto shaderPaths = platforms::ShaderPaths(vert, frag, nullptr);

		m_shader.initialize(shaderPaths);

		{ // setup Camera uniform block
			auto& camera = m_shader.createShaderBufferStorage();
			std::vector<UniformItem> cameraItems{ ut_u_Model, ut_u_MVP, ut_u_CameraPos };

			camera.initialize(ub_Camera, std::move(cameraItems));
		}

		{ // setup EntityCmp uniform block
			auto& entityCmp = m_shader.createShaderBufferStorage();
			std::vector<UniformItem> entitycmpItems{ ut_u_SeparateTransform, ut_u_samplerTypes };

			entityCmp.initialize(ub_EntityCmp, std::move(entitycmpItems));
		}
		
		{ // setup Material Uniform Block
			auto& material = m_shader.createShaderBufferStorage();
			std::vector<UniformItem> materialItems{ ut_u_material, ut_u_lightSize };

			material.initialize(ub_Material, std::move(materialItems));
		}

		{ // setup TextureSmaplers Uniform Block
			auto& textureSamplers = m_shader.createShaderBufferStorage();
			std::vector<UniformItem> textureSamplersItems{ ut_u_Color };

			textureSamplers.initialize(ub_TextureSamplers, std::move(textureSamplersItems));
		}
		
		GRAPHICS_INFO("RENDERER_BATCH: initialized!");
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
			using namespace platforms::ShaderUniforms;

			m_shader.uploadUniformMat4(ub_EntityCmp, ut_u_SeparateTransform, container.getTransforms());
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

	void RendererBatch::passTexturesToShader(const RenderContainer& container) const {
		GRAPHICS_INFO("RENDERER_BATCH: passing textures data to shader!");

		using namespace platforms::ShaderUniforms;
		typedef platforms::TextureOpenGL TextureGL;

		const auto& samplerTypes = container.getSamplerTypes();
		const auto& colors = container.getColors();
		//const auto& textures = container.getTexture2D();
		//const auto& cubemaps = container.getTextureCubemap();

		m_shader.uploadUniformFloat(ub_EntityCmp, ut_u_samplerTypes, samplerTypes);
		
		std::for_each(colors.begin(), colors.end(), [&shader = m_shader](const ColorPair& color) {
			shader.uploadUniformVec3AtIndex(ub_TextureSamplers, ut_u_Color, color.first, color.second);
		});
			
		GRAPHICS_INFO("RENDERER_BATCH: passed colors to shader");	
		/*
		std::for_each(textures.begin(), textures.end(), [&shader = m_shader](const TexturePair& texture) {
			const uint32_t textureID = TextureGL::Instance()->loadTexture(texture.second);
			const uint32_t sampler = (uint32_t)texture.first;

			TextureGL::Instance()->bind2D(sampler, textureID);
			shader.uploadUniformSampler(u_Samplers2D[sampler], sampler);
		});
		
		GRAPHICS_INFO("RENDERER_BATCH: passed textures 2d to shader");
		
		std::for_each(cubemaps.begin(), cubemaps.end(), [&shader = m_shader](const TexturePair& texture) {
			const uint32_t textureID = TextureGL::Instance()->loadCubemap(texture.second);
			const uint32_t sampler = (uint32_t)texture.first;

			TextureGL::Instance()->bindCube(sampler, textureID);
			shader.setUniformSampler(u_SamplersCube[sampler], sampler);
		});

		GRAPHICS_INFO("RENDERER_BATCH: passed cubemaps to shader!");
		*/
	}

	void RendererBatch::passLightToShader(const RenderContainer& container) const {
		GRAPHICS_INFO("RENDERER_BATCH: passing light data to shader!");

		using namespace platforms::ShaderUniforms;

		const auto& lights = container.getLights();

		m_shader.uploadUniformLightMaterial(ub_Material, ut_u_material, lights);
		m_shader.uploadUniformInt(ub_Material, ut_u_lightSize, (int32_t)lights.size());

		GRAPHICS_INFO("RENDERER_BATCH: passed light to shader!");
	}

	void RendererBatch::passCameraToShader(const RenderCamera* camera) const {
		GRAPHICS_INFO("RENDERER_BATCH: passing camera data to shader!");

		using namespace platforms::ShaderUniforms;

		m_shader.uploadUniformVec3(ub_Camera, ut_u_CameraPos, camera->getPosition());
		m_shader.uploadUniformMat4(ub_Camera, ut_u_Model, camera->getModel());
		m_shader.uploadUniformMat4(ub_Camera, ut_u_MVP, camera->getMVP());

		GRAPHICS_INFO("RENDERER_BATCH: passed camera to shader!");
	}


}
