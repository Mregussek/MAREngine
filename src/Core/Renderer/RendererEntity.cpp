/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "RendererEntity.h"
#include "Renderer.h"
#include "../../Debug/Log.h"


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

			clear();

			MAR_CORE_INFO("RENDERERENTITY: initialized!");
		}

		void RendererEntity::close() {
			m_vao.close();
			m_vbo.close();
			m_ebo.close();

			m_shaderColor.shutdown();
			m_shaderTexture2D.shutdown();
			m_shaderCubemap.shutdown();

			MAR_CORE_INFO("RENDERERENTITY: closed!");
		}

		void RendererEntity::submit(ecs::Scene* scene) {


			MAR_CORE_TRACE("RENDERERENTITY: submitted scene!");
		}

		void RendererEntity::submit(ecs::Entity& entity) {
			if (!entity.hasComponent<ecs::RenderableComponent>())
				return;

			auto& renderable = entity.getComponent<ecs::RenderableComponent>();
			auto& tran = entity.getComponent<ecs::TransformComponent>();

			if (entity.hasComponent<ecs::ColorComponent>()) {
				auto& color = entity.getComponent<ecs::ColorComponent>();

				submitTransform(m_transformsColor, tran);
				submitVerticesIndices(renderable, m_verticesColor, m_indicesColor, m_indicesMaxColor, m_counterColor, m_stride);

				m_samplersColors.push_back(color);
				m_counterColor++;
			}
			else if (entity.hasComponent<ecs::Texture2DComponent>()) {
				auto& tex = entity.getComponent<ecs::Texture2DComponent>();

				submitTransform(m_transformsTexture2D, tran);
				submitVerticesIndices(renderable, m_verticesTexture2D, m_indicesTexture2D, m_indicesMaxTexture2D, m_counterTexture2D, m_stride);

				m_samplersTexture2D.push_back(m_counterTexture2D);
				m_counterTexture2D++;
			}
			else if (entity.hasComponent<ecs::TextureCubemapComponent>()) {
				auto& cube = entity.getComponent<ecs::TextureCubemapComponent>();
				
				submitTransform(m_transformsCubemap, tran);
				submitVerticesIndices(renderable, m_verticesCubemap, m_indicesCubemap, m_indicesMaxCubemap, m_counterCubemap, m_stride);
				
				m_samplersCubemap.push_back(m_counterCubemap);
				m_counterCubemap++;
			}

			MAR_CORE_TRACE("RENDERERENTITY: submitted Entity!");
		}

		void RendererEntity::submitTransform(std::vector<maths::mat4>& transforms, maths::mat4& transform) {
			transforms.push_back(transform);

			MAR_CORE_TRACE("RENDERERENTITY: submitted transform component!");
		}

		void RendererEntity::submitVerticesIndices(ecs::RenderableComponent& ren, std::vector<float>& vertices, 
			std::vector<uint32_t>& indices, uint32_t& indicesmax, int32_t& counter, uint32_t& stride)
		{
			std::vector<uint32_t> copy = ren.indices;

			ShapeManipulator::extendShapeID(ren.vertices, stride, (float)counter);
			ShapeManipulator::extendIndices(copy, indicesmax);

			vertices.insert(vertices.end(), ren.vertices.begin(), ren.vertices.end());
			indices.insert(indices.end(), copy.begin(), copy.end());

			indicesmax += vertices.size() / stride;
		
			MAR_CORE_TRACE("RENDERERENTITY: submitted renderable component!");
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

			clear();

			MAR_CORE_INFO("RENDERERENTITY: Draw calls finished for this scene!");
		}

		void RendererEntity::clear() {
			m_verticesColor.clear();
			m_indicesColor.clear();
			m_transformsColor.clear();
			m_samplersColors.clear();
			m_counterColor = 0;
			m_indicesMaxColor = 0;

			m_verticesTexture2D.clear();
			m_indicesTexture2D.clear();
			m_transformsTexture2D.clear();
			m_samplersTexture2D.clear();
			m_counterTexture2D = 0;
			m_indicesMaxTexture2D = 0;

			m_verticesCubemap.clear();
			m_indicesCubemap.clear();
			m_transformsCubemap.clear();
			m_samplersCubemap.clear();
			m_counterCubemap = 0;
			m_indicesMaxCubemap = 0;
		
			MAR_CORE_TRACE("RENDERERENTITY: called clear method!");
		}

		void RendererEntity::draw(const std::vector<float>& vertices, const std::vector<uint32_t>& indices,
			const std::vector<maths::mat4>& transforms, const std::vector<maths::vec3>& samplers, ShaderOpenGL& shader)
		{
			MAR_CORE_TRACE("RENDERER: is preparing to draw!");

			s_stats.verticesCount += vertices.size();
			s_stats.indicesCount += indices.size();

			{ // SEND ALL DATA TO SHADERS
				shader.bind();

				passLightToShader(shader, m_light);
				passCameraToShader(shader, &Camera::getCameraData());
				shader.setUniformVectorMat4("u_SeparateTransform", transforms);
				shader.setUniformVectorVec3("u_SeparateColor", samplers);
			}
			
			{ // BIND ALL NEEDED BUFFERS
				m_vao.bind();

				m_vbo.bind();
				m_vbo.updateDynamically(vertices);

				m_ebo.bind();
				m_ebo.update(indices);
			}
			
			MAR_CORE_GL_FUNC( glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr) );

			{ // UNBIND ALREADY DRAWN BUFFERS
				m_vbo.resetBuffer();
				m_vbo.unbind();

				m_ebo.resetBuffer();
				m_ebo.unbind();

				m_vao.unbind();
			}
			
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

			MAR_CORE_GL_FUNC( glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr) );

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

		void RendererEntity::passLightToShader(ShaderOpenGL& shader, Light* light) {
			shader.setUniformVector3("u_material.lightPos", light->getPosition());

			shader.setUniformVector3("u_material.ambient", light->getAmbient());
			shader.setUniformVector3("u_material.diffuse", light->getDiffuse());
			shader.setUniformVector3("u_material.specular", light->getSpecular());

			shader.setUniformVector3("u_material.ambientStrength", light->getAmbientStrength());
			shader.setUniformVector3("u_material.diffuseStrength", light->getDiffuseStrength());
			shader.setUniformVector3("u_material.specularStrength", light->getSpecularStrength());
	
			shader.setUniform1f("u_material.shininess", light->getShininess());
	
			shader.setUniform1f("u_material.constant", light->getConstant());
			shader.setUniform1f("u_material.linear", light->getLinear());
			shader.setUniform1f("u_material.quadratic", light->getQuadratic());
		}

		void RendererEntity::passCameraToShader(ShaderOpenGL& shader, CameraData* camdata) {
			shader.setUniformVector3("u_CameraPos", camdata->position);
			shader.setUniformMat4f("u_Model", camdata->model);
			shader.setUniformMat4f("u_MVP", camdata->mvp);
		}

		RendererStatistics& RendererEntity::getStatistics() { 
			return s_stats; 
		}

		void RendererEntity::clearStatistics() {
			s_stats.resetStatistics();
		}


} }
