/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "../../../Platform/OpenGL/ShaderUniforms.h"
#include "RendererTexture.h"

namespace mar {
	namespace graphics {


		void  RendererTexture::initialize(platforms::ShaderType st) {
			GRAPHICS_INFO("RENDERERENTITY_TEXTURE: Going to initialize!");

			const std::vector<uint32_t> layout{ 3, 3, 2, 1 };

			for (size_t i = 0; i < layout.size(); i++)
				m_layout.push(layout[i], PUSH_BUFFER_FLOAT);

			m_ebo.initialize(constants::maxIndexCount);
			m_vao.initialize();
			m_vbo.initialize(constants::maxVertexCount);
			m_vao.addBuffer(m_layout);

			m_shader.initialize(st);

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

			{ // SEND ALL DATA TO SHADERS
				m_shader.bind();

				passTexturesToShader(m_shader, texture_type, storage.paths, storage.samplers);
				passLightToShader(m_shader, light);
				passCameraToShader(m_shader, camera);

				m_shader.setUniformVectorMat4("u_SeparateTransform", storage.transforms);
			}

			{ // BIND ALL NEEDED BUFFERS
				m_vao.bind();
				m_vbo.bind();
				m_ebo.bind();

				m_vbo.update(storage.vertices);
				m_ebo.update(storage.indices);
			}

			MAR_CORE_GL_FUNC(glDrawElements(GL_TRIANGLES, storage.indices.size(), GL_UNSIGNED_INT, nullptr));

			{ // CLEAR AFTER DRAW CALL
				m_vbo.resetBuffer();
				m_ebo.resetBuffer();

				m_vbo.unbind();
				m_ebo.unbind();
				m_vao.unbind();

				m_texture.unbind();
			}

			GRAPHICS_INFO("RENDERERENTITY_TEXTURE: has drawn the scene!");
		}

		void RendererTexture::passTexturesToShader(
			platforms::ShaderOpenGL& shader, uint32_t texture_type,
			const std::vector<std::string>& paths, const std::vector<int32_t>& samplers)
		{
			MAR_CORE_ASSERT(paths.size() == samplers.size(), "Samplers size are not equal to paths size!");

			{ // BIND TEXTURES
				using namespace platforms::ShaderUniforms;

				if (texture_type == GL_TEXTURE_2D) {

					for (size_t i = 0; i < paths.size(); i++) {
						m_texture.bind(texture_type, samplers[i], m_texture.loadTexture(paths[i]));
						shader.setUniformSampler(u_SeparateColor[i], samplers[i]);
					}

				}
				else if (texture_type == GL_TEXTURE_CUBE_MAP) {

					for (size_t i = 0; i < paths.size(); i++) {
						m_texture.bind(texture_type, samplers[i], m_texture.loadCubemap(paths[i]));
						shader.setUniformSampler(u_SeparateColor[i], samplers[i]);
					}

				}
			}
		}

		void RendererTexture::passLightToShader(platforms::ShaderOpenGL& shader, const ecs::LightStorage& light) {
			MAR_CORE_ASSERT(light.positions.size() == light.components.size(), "Light positions are not equal to light components!");

			using namespace platforms::ShaderUniforms;

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

		void RendererTexture::passCameraToShader(platforms::ShaderOpenGL& shader, const RenderCamera& cam) {
			shader.setUniformVector3("u_CameraPos", cam.position);
			shader.setUniformMat4f("u_Model", cam.model);
			shader.setUniformMat4f("u_MVP", cam.mvp);

			GRAPHICS_TRACE("RENDERERENTITY_TEXTURE: passed camera to shader (by values)!");
		}


} }