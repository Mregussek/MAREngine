/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Shader/ShaderUniforms.h"
#include "RendererTexture.h"

namespace mar {
	namespace graphics {


		void  RendererTexture::initialize(ShaderType st) {
			GRAPHICS_INFO("RENDERERENTITY_TEXTURE: Going to initialize!");

			const std::vector<uint32_t> layout{ 3, 3, 2, 1 };

			for (size_t i = 0; i < layout.size(); i++)
				m_layout.push(layout[i], PushBuffer::PUSH_FLOAT);

			m_ebo.initialize(constants::maxIndexCount);
			m_vao.initialize();
			m_vbo.initialize(constants::maxVertexCount);
			m_vao.addBuffer(m_layout);

			m_shader.initialize(st);

			m_lastVerticesSize = 0;

			GRAPHICS_INFO("RENDERERENTITY_TEXTURE: initialized!");
		}

		void  RendererTexture::close() {
			GRAPHICS_INFO("RENDERERENTITY_TEXTURE: Going to close!");

			m_vao.close();
			m_vbo.close();
			m_ebo.close();

			m_shader.shutdown();
			m_texture.shutdown();

			GRAPHICS_INFO("RENDERERENTITY_TEXTURE: closed!");
		}

		void  RendererTexture::draw(
			const ecs::BufferStorage<int32_t>& storage, const RenderCamera& camera,
			const ecs::LightStorage& light, uint32_t texture_type
		) {
			GRAPHICS_TRACE("RENDERERENTITY_TEXTURE: is preparing to draw!");

			{ // BIND TEXTURES
				if (texture_type == GL_TEXTURE_2D) {
					uint32_t id;
					for (size_t i = 0; i < storage.paths.size(); i++) {
						id = (uint32_t)m_texture.loadTexture(storage.paths[i]);
						m_texture.bind(texture_type, storage.samplers[i], id);
					}
				}
				else if (texture_type == GL_TEXTURE_CUBE_MAP) {
					uint32_t id;
					for (size_t i = 0; i < storage.paths.size(); i++) {
						id = (uint32_t)m_texture.loadCubemap(storage.paths[i]);
						m_texture.bind(texture_type, storage.samplers[i], id);
					}
				}
			}

			{ // SEND ALL DATA TO SHADERS
				m_shader.bind();

				passLightToShader(m_shader, light);
				passCameraToShader(m_shader, camera);

				m_shader.setUniformVectorMat4("u_SeparateTransform", storage.transforms);
				m_shader.setUniformSampler("u_SeparateColor", storage.samplers);
			}

			{ // BIND ALL NEEDED BUFFERS
				m_vao.bind();
				m_vbo.bind();
				m_ebo.bind();

				if (m_lastVerticesSize != storage.vertices.size()) {
					m_vbo.updateDynamically(storage.vertices);
					m_ebo.update(storage.indices);

					m_lastVerticesSize = storage.vertices.size();
				}
			}

			MAR_CORE_GL_FUNC(glDrawElements(GL_TRIANGLES, storage.indices.size(), GL_UNSIGNED_INT, nullptr));

			{ // CLEAR AFTER DRAW CALL
				for (int32_t i = 0; i < (int32_t)storage.paths.size(); i++) {
					m_texture.bind(texture_type, storage.samplers[i], 0);
				}
			}

			GRAPHICS_INFO("RENDERERENTITY_TEXTURE: has drawn the scene!");
		}

		void RendererTexture::passLightToShader(ShaderOpenGL& shader, const ecs::LightStorage& light) {
			MAR_CORE_ASSERT(light.positions.size() == light.components.size(), "Light positions are not equal to light components!");

			using namespace ShaderUniforms;

			for (size_t i = 0; i < light.components.size(); i++) {
				shader.setUniformVector3(u_material[i].lightPos, light.positions[i]);

				shader.setUniformVector3(u_material[i].ambient, light.components[i].ambient);
				shader.setUniformVector3(u_material[i].diffuse, light.components[i].diffuse);
				shader.setUniformVector3(u_material[i].specular, light.components[i].specular);

				shader.setUniform1f(u_material[i].shininess, light.components[i].shininess);

				shader.setUniform1f(u_material[i].constant, light.components[i].constant);
				shader.setUniform1f(u_material[i].linear, light.components[i].linear);
				shader.setUniform1f(u_material[i].quadratic, light.components[i].quadratic);
			}

			shader.setUniform1i(u_materialSize, light.positions.size());

			GRAPHICS_TRACE("RENDERERENTITY_COLOR: passed light to shader!");
		}

		void RendererTexture::passCameraToShader(ShaderOpenGL& shader, const RenderCamera& cam) {
			shader.setUniformVector3("u_CameraPos", cam.position);
			shader.setUniformMat4f("u_Model", cam.model);
			shader.setUniformMat4f("u_MVP", cam.mvp);

			GRAPHICS_TRACE("RENDERERENTITY_TEXTURE: passed camera to shader (by values)!");
		}


} }