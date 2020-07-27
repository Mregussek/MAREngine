/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "RendererEntity.h"
#include "Renderer.h"


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
			m_shaderTexture2D.initialize(SHADER_ENTITY_TEXTURE2D);
			m_shaderCubemap.initialize(SHADER_ENTITY_CUBEMAP);

			m_stride = 3 + 3 + 2 + 1;

			m_counterColor = 0;
			m_counterTexture2D = 0;
			m_counterCubemap = 0;

			m_indicesMaxColor = 0;
			m_indicesMaxTexture2D = 0;
			m_indicesMaxCubemap = 0;
		}

		void RendererEntity::close() {
			m_vao.close();
			m_vbo.close();
			m_ebo.close();

			m_shaderColor.shutdown();
			m_shaderTexture2D.shutdown();
			m_shaderCubemap.shutdown();
		}

		void RendererEntity::submit(ecs::Scene* scene) {
			for (auto& entity : scene->getEntities())
				submit(entity);
		}

		void RendererEntity::submit(ecs::Entity entity) {
			if (!entity.hasComponent<ecs::RenderableComponent>())
				return;

			if (entity.hasComponent<ecs::ColorComponent>()) {
				auto& renderable = entity.getComponent<ecs::RenderableComponent>();
				auto& color = entity.getComponent<ecs::ColorComponent>();
				auto& tran = entity.getComponent<ecs::TransformComponent>();

				m_transformsColor.push_back(tran);

				std::vector<uint32_t> indices = renderable.indices;

				ShapeManipulator::extendShapeID(renderable.vertices, m_stride, (float)m_counterColor);
				ShapeManipulator::extendIndices(indices, m_indicesMaxColor);

				m_verticesColor.insert(m_verticesColor.end(), renderable.vertices.begin(), renderable.vertices.end());
				m_indicesColor.insert(m_indicesColor.end(), indices.begin(), indices.end());

				m_samplersColors.push_back(color);
				m_counterColor++;

				m_indicesMaxColor += m_verticesColor.size() / m_stride;
			}
			else if (entity.hasComponent<ecs::Texture2DComponent>()) {
				auto& renderable = entity.getComponent<ecs::RenderableComponent>();
				auto& tex = entity.getComponent<ecs::Texture2DComponent>();
				auto& tran = entity.getComponent<ecs::TransformComponent>();

				m_transformsTexture2D.push_back(tran);

				std::vector<uint32_t> indices = renderable.indices;
				ShapeManipulator::extendShapeID(renderable.vertices, m_stride, (float)m_counterTexture2D);
				ShapeManipulator::extendIndices(indices, m_indicesMaxTexture2D);

				m_verticesTexture2D.insert(m_verticesTexture2D.end(), renderable.vertices.begin(), renderable.vertices.end());
				m_indicesTexture2D.insert(m_indicesTexture2D.end(), indices.begin(), indices.end());

				m_samplersTexture2D.push_back(m_counterTexture2D);
				m_counterTexture2D++;

				m_indicesMaxTexture2D += m_verticesTexture2D.size() / m_stride;
			}
			else if (entity.hasComponent<ecs::TextureCubemapComponent>()) {
				auto& renderable = entity.getComponent<ecs::RenderableComponent>();
				auto& cube = entity.getComponent<ecs::TextureCubemapComponent>();
				auto& tran = entity.getComponent<ecs::TransformComponent>();

				m_transformsCubemap.push_back(tran);

				std::vector<uint32_t> indices = renderable.indices;
				ShapeManipulator::extendShapeID(renderable.vertices, m_stride, (float)m_counterCubemap);
				ShapeManipulator::extendIndices(indices, m_indicesMaxCubemap);

				m_verticesCubemap.insert(m_verticesCubemap.end(), renderable.vertices.begin(), renderable.vertices.end());
				m_indicesCubemap.insert(m_indicesCubemap.end(), indices.begin(), indices.end());

				m_samplersCubemap.push_back(m_counterCubemap);
				m_counterCubemap++;

				m_indicesMaxCubemap += m_verticesCubemap.size() / m_stride;
			}
		}

		void RendererEntity::update() {
			if (!m_verticesColor.empty()) {
				draw(m_verticesColor, m_indicesColor, m_transformsColor, m_samplersColors, m_shaderColor);
			}

			if (!m_verticesTexture2D.empty()) {
				draw(m_verticesTexture2D, m_indicesTexture2D, m_transformsTexture2D, m_samplersTexture2D, m_shaderTexture2D);
			}

			if (!m_verticesCubemap.empty()) {
				draw(m_verticesCubemap, m_indicesCubemap, m_transformsCubemap, m_samplersCubemap, m_shaderCubemap);
			}
		}

		void RendererEntity::clear() {
			m_verticesColor.clear();
			m_indicesColor.clear();
			m_transformsColor.clear();
			m_samplersColors.clear();

			m_verticesCubemap.clear();
			m_indicesCubemap.clear();
			m_transformsCubemap.clear();
			m_samplersCubemap.clear();

			m_verticesTexture2D.clear();
			m_indicesTexture2D.clear();
			m_transformsTexture2D.clear();
			m_samplersTexture2D.clear();

			m_counterColor = 0;
			m_counterTexture2D = 0;
			m_counterCubemap = 0;

			m_indicesMaxColor = 0;
			m_indicesMaxTexture2D = 0;
			m_indicesMaxCubemap = 0;
		}

		void RendererEntity::draw(const std::vector<float>& vertices, const std::vector<uint32_t>& indices,
			const std::vector<maths::mat4>& transforms, const std::vector<maths::vec3>& samplers, ShaderOpenGL& shader)
		{
			MAR_CORE_TRACE("RENDERER: is preparing to draw!");

			s_stats.verticesCount += vertices.size();
			s_stats.indicesCount += indices.size();

			shader.bind();

			shader.setUniformMat4f("u_MVP", *m_mvp);
			shader.setUniformVectorMat4("u_SeparateTransform", transforms);
			shader.setUniformVectorVec3("u_SeparateColor", samplers);

			m_vao.bind();

			m_vbo.bind();
			m_vbo.updateDynamically(vertices);

			m_ebo.bind();
			m_ebo.update(indices);

			MAR_CORE_GL_FUNC(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr));

			m_vbo.resetBuffer();
			m_vbo.unbind();

			m_ebo.resetBuffer();
			m_ebo.unbind();

			m_vao.unbind();

			s_stats.drawCallsCount += 1;
			s_stats.trianglesCount = s_stats.indicesCount / 3;

			MAR_CORE_INFO("RENDERER: has drawn the scene!");
			MAR_CORE_INFO("Draw Call: " + std::to_string(s_stats.drawCallsCount));
		}

		void RendererEntity::draw(const std::vector<float>& vertices, const std::vector<uint32_t>& indices,
			const std::vector<maths::mat4>& transforms, const std::vector<int32_t>& samplers, ShaderOpenGL& shader)
		{
			MAR_CORE_TRACE("RENDERER: is preparing to draw!");

			s_stats.verticesCount += vertices.size();
			s_stats.indicesCount += indices.size();

			shader.bind();

			/////! TODO: write shaders for textures2D and cubemap, send uniforms


			m_vao.bind();

			m_vbo.bind();
			m_vbo.updateDynamically(vertices);

			m_ebo.bind();
			m_ebo.update(indices);

			MAR_CORE_GL_FUNC(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr));

			m_vbo.resetBuffer();
			m_vbo.unbind();

			m_ebo.resetBuffer();
			m_ebo.unbind();

			m_vao.unbind();

			s_stats.drawCallsCount += 1;
			s_stats.trianglesCount = s_stats.indicesCount / 3;

			MAR_CORE_INFO("RENDERER: has drawn the scene!");
			MAR_CORE_INFO("Draw Call: " + std::to_string(s_stats.drawCallsCount));
		}


} }
