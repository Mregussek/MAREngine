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
#include "../GraphicsLogs.h"
#include "../../../Platform/OpenGL/DrawingOpenGL.h"
#include "../../../Platform/OpenGL/ShaderUniforms.h"
#include "../../../Platform/OpenGL/TextureOpenGL.h"


namespace mar {
	namespace graphics {


		void RendererBatch::initialize() {
			GRAPHICS_INFO("RENDERER_BATCH: going to initialize!");

			uint32_t max_vertex_count = 500000;
			uint32_t max_index_count = 500000 / 2;

			m_buffers.initialize(max_vertex_count, max_index_count);
			m_shader.initialize();
		
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
			{
				m_shader.bind();

				m_shader.setUniformMat4("u_SeparateTransform", render_pip.m_transforms);
				passTexturesToShader(render_pip);
				passLightToShader(render_pip);
				passCameraToShader(render_pip);
			}
			{
				m_buffers.bind();
				m_buffers.update(render_pip.m_vertices, render_pip.m_indices);
			
				platforms::DrawingOpenGL::drawTriangles(render_pip.m_indices.size());
			
				m_buffers.reset();
				m_buffers.unbind();

				platforms::TextureOpenGL::Instance()->unbind(render_pip.m_samplerTypes);
			}

			render_pip.s_statistics.drawCallsCount += 1;
			render_pip.s_statistics.shapesCount += render_pip.m_transforms.size();
			render_pip.s_statistics.verticesCount += render_pip.m_vertices.size();
			render_pip.s_statistics.indicesCount += render_pip.m_indices.size();
			render_pip.s_statistics.trianglesCount += render_pip.m_indices.size() / 3;

			GRAPHICS_INFO("RENDERER_BATCH: drawn data given from render pipeline!");
		}

		void RendererBatch::passTexturesToShader(RenderPipeline& ren) {
			GRAPHICS_INFO("RENDERER_BATCH: passing textures data to shader!");

			using namespace platforms::ShaderUniforms;
			typedef platforms::TextureOpenGL TextureGL;

			m_shader.setUniformFloat("u_samplerTypes", ren.m_samplerTypes);

			static uint32_t tex_id = 0;
			static uint32_t sampler = 0;

			for (size_t i = 0; i < ren.m_colors.size(); i++) {
				sampler = (uint32_t)ren.m_colors[i].first;
				m_shader.setUniformVec3(u_SamplersColor[sampler], ren.m_colors[i].second);
			}
				
			GRAPHICS_INFO("RENDERER_BATCH: passed colors to shader");	

			for (size_t i = 0; i < ren.m_tex2D.size(); i++) {
				tex_id = TextureGL::Instance()->loadTexture(ren.m_tex2D[i].second);
				sampler = (uint32_t)ren.m_tex2D[i].first;

				TextureGL::Instance()->bind2D(sampler, tex_id);
				m_shader.setUniformSampler(u_Samplers2D[sampler], sampler);
			}
			
			GRAPHICS_INFO("RENDERER_BATCH: passed textures 2d to shader");

			for (size_t i = 0; i < ren.m_cubes.size(); i++) {
				tex_id = TextureGL::Instance()->loadCubemap(ren.m_cubes[i].second);
				sampler = (size_t)ren.m_cubes[i].first;

				TextureGL::Instance()->bindCube(sampler, tex_id);
				m_shader.setUniformSampler(u_SamplersCube[sampler], sampler);
			}

			GRAPHICS_INFO("RENDERER_BATCH: passed cubemaps to shader!");
		}

		void RendererBatch::passLightToShader(RenderPipeline& ren) {
			GRAPHICS_INFO("RENDERER_BATCH: passing light data to shader!");

			using namespace platforms::ShaderUniforms;

			for (size_t i = 0; i < ren.m_lights.size(); i++) {
				m_shader.setUniformVec3(u_material[i].lightPos, ren.m_lights[i].first);
		
				m_shader.setUniformVec3(u_material[i].ambient, ren.m_lights[i].second.ambient);
				m_shader.setUniformVec3(u_material[i].diffuse, ren.m_lights[i].second.diffuse);
				m_shader.setUniformVec3(u_material[i].specular, ren.m_lights[i].second.specular);
		
				m_shader.setUniformFloat(u_material[i].shininess, ren.m_lights[i].second.shininess);
	
				m_shader.setUniformFloat(u_material[i].constant, ren.m_lights[i].second.constant);
				m_shader.setUniformFloat(u_material[i].linear, ren.m_lights[i].second.linear);
				m_shader.setUniformFloat(u_material[i].quadratic, ren.m_lights[i].second.quadratic);
			}

			m_shader.setUniformInt(u_materialSize, ren.m_lights.size());

			GRAPHICS_INFO("RENDERER_BATCH: passed light to shader!");
		}

		void RendererBatch::passCameraToShader(RenderPipeline& ren) {
			GRAPHICS_INFO("RENDERER_BATCH: passing camera data to shader!");

			m_shader.setUniformVec3("u_CameraPos", ren.m_camera->position);
			m_shader.setUniformMat4("u_Model", ren.m_camera->model);
			m_shader.setUniformMat4("u_MVP", ren.m_camera->mvp);

			GRAPHICS_INFO("RENDERER_BATCH: passed camera to shader!");
		}


} }