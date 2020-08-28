/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Shader/ShaderUniforms.h"
#include "RendererColor.h"

namespace mar {
	namespace graphics {


		void RendererColor::initialize() {
			GRAPHICS_INFO("RENDERERENTITY_COLOR: Going to initialize!");

			const std::vector<uint32_t> layout{ 3, 3, 2, 1 };

			for (size_t i = 0; i < layout.size(); i++)
				m_layout.push(layout[i], PushBuffer::PUSH_FLOAT);

			m_ebo.initialize(constants::maxIndexCount);
			m_vao.initialize();
			m_vbo.initialize(constants::maxVertexCount);
			m_vao.addBuffer(m_layout);

			m_shader.initialize(SHADER_ENTITY_COLOR);

			GRAPHICS_INFO("RENDERERENTITY_COLOR: initialized!");
		}

		void RendererColor::close() {
			GRAPHICS_INFO("RENDERERENTITY_COLOR: Going to close!");

			m_vao.close();
			m_vbo.close();
			m_ebo.close();

			m_shader.shutdown();

			GRAPHICS_INFO("RENDERERENTITY_COLOR: closed!");
		}

		void RendererColor::draw(
			const ecs::BufferStorage<maths::vec3>& storage, const RenderCamera& camera, const ecs::LightStorage& light)
		{
			GRAPHICS_TRACE("RENDERERENTITY_COLOR: is preparing to draw!");

			{ // SEND ALL DATA TO SHADERS
				m_shader.bind();

				passLightToShader(m_shader, light);
				passCameraToShader(m_shader, camera);

				m_shader.setUniformVectorMat4("u_SeparateTransform", storage.transforms);
				m_shader.setUniformVectorVec3("u_SeparateColor", storage.samplers);
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
			}

			GRAPHICS_INFO("RENDERERENTITY_COLOR: has drawn the scene!");
		}

		void RendererColor::passCameraToShader(ShaderOpenGL& shader, const RenderCamera& cam) {
			shader.setUniformVector3("u_CameraPos", cam.position);
			shader.setUniformMat4f("u_Model", cam.model);
			shader.setUniformMat4f("u_MVP", cam.mvp);

			GRAPHICS_TRACE("RENDERERENTITY_COLOR: passed camera to shader (by values)!");
		}

		void RendererColor::passLightToShader(ShaderOpenGL& shader, const ecs::LightStorage& light) {
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


} }