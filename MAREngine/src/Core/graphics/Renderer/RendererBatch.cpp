/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "RendererBatch.h"
#include "../../../Platform/OpenGL/ShaderUniforms.h"


namespace mar {
	namespace graphics {


		void RendererBatch::initialize() {
			GRAPHICS_INFO("RENDERER_BATCH: going to initialize!");

			uint32_t max_vertex_count = 500000;
			uint32_t max_index_count = 500000 / 2;

			m_buffers.initialize(max_vertex_count, max_index_count);
			m_buffers.processLayout();

			m_shader.initialize(SHADER_ENTITY_BATCHER);
		
			GRAPHICS_INFO("RENDERER_BATCH: initialized!");
		}

		void RendererBatch::close() {
			GRAPHICS_INFO("RENDERER_BATCH: going to close!");

			m_buffers.close();
			m_shader.shutdown();

			GRAPHICS_INFO("RENDERER_BATCH: closed!");
		}

		void RendererBatch::draw(RenderPipeline& render_pip) {
			GRAPHICS_TRACE("RENDERER_BATCH: going to draw render pipeline!");
			{
				m_shader.bind();

				m_shader.setUniformVectorMat4("u_SeparateTransform", render_pip.m_transforms);
				passTexturesToShader(render_pip);
				passLightToShader(render_pip);
				passCameraToShader(render_pip);
			}
			
			{
				m_buffers.bind();
				m_buffers.updateBuffers(render_pip.m_vertices, render_pip.m_indices);
			
				platforms::DrawingOpenGL::drawTriangles(render_pip.m_indices.size());
			
				m_buffers.resetBuffers();
				m_buffers.unbind();

				m_texture.unbind(render_pip.m_samplerTypes);
			}

			GRAPHICS_INFO("RENDERER_BATCH: drawn data given from render pipeline!");
		}

		void RendererBatch::passTexturesToShader(RenderPipeline& ren) {
			GRAPHICS_INFO("RENDERER_BATCH: passing textures data to shader!");

			using namespace platforms::ShaderUniforms;

			m_shader.setUniformFloat("u_samplerTypes", ren.m_samplerTypes);

			for (size_t i = 0; i < ren.m_colors.size(); i++) 
				m_shader.setUniformVector3(u_SamplersColor[(size_t)ren.m_colors[i].first], ren.m_colors[i].second);
			
			static uint32_t tex_id = 0;
			static uint32_t sampler = 0;

			for (size_t i = 0; i < ren.m_tex2D.size(); i++) {
				tex_id = m_texture.loadTexture(ren.m_tex2D[i].second);
				sampler = (uint32_t)ren.m_tex2D[i].first;

				m_texture.bind2D(sampler, tex_id);
				m_shader.setUniformSampler(u_Samplers2D[sampler], sampler);
			}
				
			for (size_t i = 0; i < ren.m_cubes.size(); i++) {
				tex_id = m_texture.loadCubemap(ren.m_cubes[i].second);
				sampler = (size_t)ren.m_cubes[i].first;

				m_texture.bindCube(sampler, tex_id);
				m_shader.setUniformSampler(u_SamplersCube[sampler], sampler);
			}

			GRAPHICS_INFO("RENDERER_BATCH: passed textures to shader!");
		}

		void RendererBatch::passLightToShader(RenderPipeline& ren) {
			GRAPHICS_INFO("RENDERER_BATCH: passing light data to shader!");

			using namespace platforms::ShaderUniforms;

			for (size_t i = 0; i < ren.m_lights.size(); i++) {
				m_shader.setUniformVector3(u_material[i].lightPos, ren.m_lights[i].first);
		
				m_shader.setUniformVector3(u_material[i].ambient, ren.m_lights[i].second.ambient);
				m_shader.setUniformVector3(u_material[i].diffuse, ren.m_lights[i].second.diffuse);
				m_shader.setUniformVector3(u_material[i].specular, ren.m_lights[i].second.specular);
		
				m_shader.setUniform1f(u_material[i].shininess, ren.m_lights[i].second.shininess);
	
				m_shader.setUniform1f(u_material[i].constant, ren.m_lights[i].second.constant);
				m_shader.setUniform1f(u_material[i].linear, ren.m_lights[i].second.linear);
				m_shader.setUniform1f(u_material[i].quadratic, ren.m_lights[i].second.quadratic);
			}

			m_shader.setUniform1i(u_materialSize, ren.m_lights.size());

			GRAPHICS_INFO("RENDERER_BATCH: passed light to shader!");
		}

		void RendererBatch::passCameraToShader(RenderPipeline& ren) {
			GRAPHICS_INFO("RENDERER_BATCH: passing camera data to shader!");

			m_shader.setUniformVector3("u_CameraPos", ren.m_camera->position);
			m_shader.setUniformMat4f("u_Model", ren.m_camera->model);
			m_shader.setUniformMat4f("u_MVP", ren.m_camera->mvp);

			GRAPHICS_INFO("RENDERER_BATCH: passed camera to shader!");
		}


} }