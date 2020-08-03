/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "RendererEntity.h"
#include "Renderer.h"
#include "../GraphicsLogs.h"
#include "Shader/ShaderUniforms.h"


namespace mar {
	namespace graphics {


		RendererStatistics RendererEntity::s_stats;

		void RendererEntity::initialize() {
			const std::vector<uint32_t> layout{ 3, 3, 2, 1 };

			for (size_t i = 0; i < layout.size(); i++)
				m_layout.push(layout[i], PushBuffer::PUSH_FLOAT);

			m_ebo.initialize(constants::maxIndexCount);
			m_vao.initialize();
			m_vbo.initialize(constants::maxVertexCount);
			m_vao.addBuffer(m_layout);

			m_shaderColor.initialize(SHADER_ENTITY_COLOR);
			m_shaderTextures2D.initialize(SHADER_ENTITY_TEXTURE2D);
			m_shaderCubemaps.initialize(SHADER_ENTITY_CUBEMAP);

			GRAPHICS_INFO("RENDERERENTITY: initialized!");
		}

		void RendererEntity::close() {
			m_vao.close();
			m_vbo.close();
			m_ebo.close();

			m_shaderColor.shutdown();
			m_shaderTextures2D.shutdown();
			m_shaderCubemaps.shutdown();

			GRAPHICS_INFO("RENDERERENTITY: closed!");
		}

		void RendererEntity::update(ecs::Scene* scene) {
			auto& colors = scene->getColorsStorage();
			auto& cubemaps = scene->getCubemapStorage();
			auto& textures = scene->getTexturesStorage();

			if (!colors.vertices.empty()) {
				draw(colors,
					 scene->getRenderCamera(),
					 scene->getLightStorage(),
					 m_shaderColor
				);
			}

			if(!cubemaps.vertices.empty()) {
				draw(cubemaps,
					 scene->getRenderCamera(),
					 scene->getLightStorage(), 
					 scene->getTextureInstance(),
					 m_shaderCubemaps, GL_TEXTURE_CUBE_MAP
				);
			}

			if (!textures.vertices.empty()) {
				draw(textures,
					 scene->getRenderCamera(),
					 scene->getLightStorage(), 
					 scene->getTextureInstance(),
					 m_shaderTextures2D, GL_TEXTURE_2D
				);
			}

			GRAPHICS_INFO("RENDERERENTITY: Draw calls finished for this scene!");
		}

		void RendererEntity::draw(
			const ecs::SceneStorage<maths::vec3>& storage, const RenderCamera& camera,
			const ecs::LightStorage& light, ShaderOpenGL& shader)
		{
			GRAPHICS_TRACE("RENDERER: is preparing to draw!");

			{ // SEND ALL DATA TO SHADERS
				shader.bind();

				passLightToShader(shader, light);
				passCameraToShader(shader, camera);

				shader.setUniformVectorMat4("u_SeparateTransform", storage.transforms);
				shader.setUniformVectorVec3("u_SeparateColor", storage.samplers);
			}
			
			{ // BIND ALL NEEDED BUFFERS
				m_vao.bind();

				m_vbo.bind();
				m_vbo.updateDynamically(storage.vertices);

				m_ebo.bind();
				m_ebo.update(storage.indices);
			}
			
			MAR_CORE_GL_FUNC( glDrawElements(GL_TRIANGLES, storage.indices.size(), GL_UNSIGNED_INT, nullptr) );

			{ // UNBIND ALREADY DRAWN BUFFERS
				m_vbo.resetBuffer();
				m_vbo.unbind();

				m_ebo.resetBuffer();
				m_ebo.unbind();

				m_vao.unbind();
			}

			s_stats.verticesCount += storage.vertices.size();
			s_stats.indicesCount += storage.indices.size();
			s_stats.drawCallsCount += 1;
			s_stats.trianglesCount = s_stats.indicesCount / 3;

			GRAPHICS_INFO("RENDERER: has drawn the scene!");
			GRAPHICS_INFO("Draw Call: " + std::to_string(s_stats.drawCallsCount));
		}

		void RendererEntity::draw(const ecs::SceneStorage<int32_t>& storage, 
			const RenderCamera& camera, const ecs::LightStorage& light, const TextureOpenGL& texture,
			ShaderOpenGL& shader, const int32_t& texture_type)
		{
			GRAPHICS_TRACE("RENDERER: is preparing to draw!");

			{ // BIND TEXTURES
				if(texture_type == GL_TEXTURE_2D)
					for (int32_t i = 0; i < (int32_t)storage.samplers.size(); i++)
						texture.bind(texture_type, storage.samplers[i], texture.getTexture(storage.paths[i]));
				else if(texture_type == GL_TEXTURE_CUBE_MAP)
					for (int32_t i = 0; i < (int32_t)storage.samplers.size(); i++)
						texture.bind(texture_type, storage.samplers[i], texture.getCubemap(storage.paths[i]));
			}

			{ // SEND ALL DATA TO SHADERS
				shader.bind();

				passLightToShader(shader, light);
				passCameraToShader(shader, camera);

				shader.setUniformVectorMat4("u_SeparateTransform", storage.transforms);
				shader.setUniformSampler("u_SeparateColor", storage.samplers);
			}

			{ // BIND ALL NEEDED BUFFERS
				m_vao.bind();

				m_vbo.bind();
				m_vbo.updateDynamically(storage.vertices);

				m_ebo.bind();
				m_ebo.update(storage.indices);
			}

			MAR_CORE_GL_FUNC(glDrawElements(GL_TRIANGLES, storage.indices.size(), GL_UNSIGNED_INT, nullptr));

			{ // UNBIND ALREADY DRAWN BUFFERS
				m_vbo.resetBuffer();
				m_vbo.unbind();

				m_ebo.resetBuffer();
				m_ebo.unbind();

				m_vao.unbind();
			}

			s_stats.verticesCount += storage.vertices.size();
			s_stats.indicesCount += storage.indices.size();

			s_stats.drawCallsCount += 1;
			s_stats.trianglesCount = s_stats.indicesCount / 3;

			GRAPHICS_INFO("RENDERER: has drawn the scene!");
			GRAPHICS_INFO("Draw Call: " + std::to_string(s_stats.drawCallsCount));
		}

		void RendererEntity::passLightToShader(ShaderOpenGL& shader, const ecs::LightStorage& light) {
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

			GRAPHICS_TRACE("RENDERERENTITY: passed light to shader!");
		}

		void RendererEntity::passCameraToShader(ShaderOpenGL& shader, const RenderCamera& cam) {
			shader.setUniformVector3("u_CameraPos", cam.position);
			shader.setUniformMat4f("u_Model", cam.model);
			shader.setUniformMat4f("u_MVP", cam.mvp);
			
			GRAPHICS_TRACE("RENDERERENTITY: passed camera to shader (by values)!");
		}

		RendererStatistics& RendererEntity::getStatistics() { 
			return s_stats; 
		}

		void RendererEntity::clearStatistics() {
			s_stats.resetStatistics();

			GRAPHICS_INFO("RENDERERENTITY: reseting renderer statistics!");
		}


} }
