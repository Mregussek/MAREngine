/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_RENDERER_ENTITY_H
#define MAR_ENGINE_RENDERER_ENTITY_H

#include "../../mar.h"
#include "../Scene/Entity/Entity.h"
#include "../Scene/Component/Components.h"
#include "RendererOpenGL.h"



namespace mar {
	namespace graphics {


		class RendererEntity {
			// --- BUFFERS --- //
			VertexBufferOpenGL m_vbo;
			VertexBufferLayoutOpenGL m_layout;
			VertexArrayOpenGL m_vao;
			ElementBufferOpenGL m_ebo;
			
			ShaderOpenGL m_shaderColor;
			ShaderOpenGL m_shaderTexture2D;
			ShaderOpenGL m_shaderCubemap;

			// --- STORAGE --- //
			std::vector<float> m_verticesColor;
			std::vector<uint32_t> m_indicesColor;
			std::vector<maths::vec3> m_samplersColors;

			std::vector<float> m_verticesTexture2D;
			std::vector<uint32_t> m_indicesTexture2D;
			std::vector<int32_t> m_samplersTexture2D;

			std::vector<float> m_verticesCubemap;
			std::vector<uint32_t> m_indicesCubemap;
			std::vector<int32_t> m_samplersCubemap;
			
			// --- OTHER --- //
			int32_t m_counterTexture2D;
			int32_t m_counterCubemap;
			uint32_t m_indicesMaxColor;
			uint32_t m_indicesMaxTexture2D;
			uint32_t m_indicesMaxCubemap;
			uint32_t m_stride;
			static RendererStatistics s_stats;

		public:
			
			void initialize() {
				const std::vector<uint32_t> layout{ 3, 3, 2, 1, 1 };

				for (size_t i = 0; i < layout.size(); i++)
					m_layout.push(layout[i], PushBuffer::PUSH_FLOAT);

				m_ebo.initialize(constants::maxIndexCount);
				m_vao.initialize();
				m_vbo.initialize(constants::maxVertexCount);
				m_vao.addBuffer(m_layout);

				m_shaderColor.initialize(SHADER_ENTITY_COLOR);
				m_shaderTexture2D.initialize(SHADER_ENTITY_TEXTURE2D);
				m_shaderCubemap.initialize(SHADER_ENTITY_CUBEMAP);

				m_stride = 3 + 3 + 2 + 1 + 1;
				m_counterTexture2D = 0;
				m_counterCubemap = 0;

				m_indicesMaxColor = 0;
				m_indicesMaxTexture2D = 0;
				m_indicesMaxCubemap = 0;
			}

			void close() {
				m_vao.close();
				m_vbo.close();
				m_ebo.close();

				m_shaderColor.shutdown();
				m_shaderTexture2D.shutdown();
				m_shaderCubemap.shutdown();
			}

			void submit(ecs::Entity entity) {
				if (!entity.hasComponent<ecs::RenderableComponent>())
					return;

				if (entity.hasComponent<ecs::ColorComponent>()) {
					auto& renderable = entity.getComponent<ecs::RenderableComponent>();
					auto& color = entity.getComponent<ecs::ColorComponent>();

					std::vector<uint32_t> indices = renderable.indices;
					ShapeManipulator::extendIndices(indices, m_indicesMaxColor);

					m_verticesColor.insert(m_verticesColor.end(), 
						renderable.vertices.begin(), renderable.vertices.end());
					m_indicesColor.insert(m_indicesColor.end(), 
						indices.begin(), indices.end());

					m_samplersColors.push_back(color);
					m_indicesMaxColor += m_verticesColor.size() / m_stride;
				}
				else if (entity.hasComponent<ecs::Texture2DComponent>()) {
					auto& renderable = entity.getComponent<ecs::RenderableComponent>();
					auto& tex = entity.getComponent<ecs::Texture2DComponent>();

					std::vector<uint32_t> indices = renderable.indices;
					ShapeManipulator::extendBothIDs(renderable.vertices, m_stride, (float)m_counterTexture2D);
					ShapeManipulator::extendIndices(indices, m_indicesMaxTexture2D);

					m_verticesTexture2D.insert(m_verticesTexture2D.end(),
						renderable.vertices.begin(), renderable.vertices.end());
					m_indicesTexture2D.insert(m_indicesTexture2D.end(),
						indices.begin(), indices.end());

					m_samplersTexture2D.push_back(m_counterTexture2D);
					m_counterTexture2D++;

					m_indicesMaxTexture2D += m_verticesTexture2D.size() / m_stride;
				}
				else if (entity.hasComponent<ecs::TextureCubemapComponent>()) {
					auto& renderable = entity.getComponent<ecs::RenderableComponent>();

					std::vector<uint32_t> indices = renderable.indices;
					ShapeManipulator::extendBothIDs(renderable.vertices, m_stride, (float)m_counterCubemap);
					ShapeManipulator::extendIndices(indices, m_indicesMaxCubemap);

					m_verticesCubemap.insert(m_verticesCubemap.end(),
						renderable.vertices.begin(), renderable.vertices.end());
					m_indicesCubemap.insert(m_indicesCubemap.end(),
						indices.begin(), indices.end());

					m_samplersCubemap.push_back(m_counterCubemap);
					m_counterCubemap++;

					m_indicesMaxCubemap += m_verticesCubemap.size() / m_stride;
				}
			}

			void update() {
				if (!m_verticesColor.empty()) {
					draw(m_verticesColor, m_indicesColor, m_samplersColors, m_shaderColor);
				}
					
				if (!m_verticesTexture2D.empty()) {
					draw(m_verticesTexture2D, m_indicesTexture2D, m_samplersTexture2D, m_shaderTexture2D);
				}

				if (!m_verticesCubemap.empty()) {
					draw(m_verticesCubemap, m_indicesCubemap, m_samplersCubemap, m_shaderCubemap);
				}
			}

			void clear() {
				m_verticesColor.clear();
				m_indicesColor.clear();
				m_samplersColors.clear();

				m_verticesCubemap.clear();
				m_indicesCubemap.clear();
				m_samplersCubemap.clear();

				m_verticesTexture2D.clear();
				m_indicesTexture2D.clear();
				m_samplersTexture2D.clear();

				m_counterTexture2D = 0;
				m_counterCubemap = 0;

				m_indicesMaxColor = 0;
				m_indicesMaxTexture2D = 0;
				m_indicesMaxCubemap = 0;
			}

		private:
			void draw(const std::vector<float>& vertices, const std::vector<uint32_t>& indices,
				const std::vector<maths::vec3>& samplers, ShaderOpenGL& shader) {

				//////! TODO: write function for colors
			}

			void draw(const std::vector<float>& vertices, const std::vector<uint32_t>& indices,
							const std::vector<int32_t>& samplers, ShaderOpenGL& shader) 
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
		};


} }


#endif // !MAR_ENGINE_RENDERER_ENTITY_H
