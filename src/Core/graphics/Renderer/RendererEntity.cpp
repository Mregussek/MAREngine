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
			//m_shaderTexture2D.initialize(SHADER_ENTITY_TEXTURE2D);
			//m_shaderCubemap.initialize(SHADER_ENTITY_CUBEMAP);

			m_stride = 3 + 3 + 2 + 1;

			m_lastSize = 0;

			clear();

			GRAPHICS_INFO("RENDERERENTITY: initialized!");
		}

		void RendererEntity::close() {
			m_vao.close();
			m_vbo.close();
			m_ebo.close();

			m_shaderColor.shutdown();
			//m_shaderTexture2D.shutdown();
			//m_shaderCubemap.shutdown();

			GRAPHICS_INFO("RENDERERENTITY: closed!");
		}

		void RendererEntity::submit(ecs::Scene* scene) {
			if (m_lastSize == scene->entities.size()) {
				if (scene->updatedBuffers)
					goto resubmit_all;

				if (scene->updatedTransforms)
					updateTransforms(scene);

				scene->updatedTransforms = false;

				if (scene->updatedCamera)
					updateCamera(scene->scene_camera.projection, scene->scene_camera.view, 
						scene->scene_camera.model, scene->scene_camera.position);

				scene->updatedCamera = false;

				if (scene->updatedColors)
					updateColors(scene);

				scene->updatedColors = false;

				if (scene->updatedLight)
					updateLight(scene);
				
				scene->updatedLight = false;

				m_lastSizeSet = false;
				return;
			}
				
		resubmit_all:

			scene->updatedBuffers = false;
			
			m_lastSize = scene->entities.size();
			m_lastSizeSet = true;

			clear();

			updateLight(scene);
			updateCamera(scene->scene_camera.projection, scene->scene_camera.view,
				scene->scene_camera.model, scene->scene_camera.position);

			for (auto& entity : scene->entities)
				submitEntity(entity);

			GRAPHICS_TRACE("RENDERERENTITY: submitted scene!");
		}

		void RendererEntity::submitEntity(ecs::Entity& entity) {
			if (!entity.hasComponent<ecs::RenderableComponent>())
				return;

			auto& renderable = entity.getComponent<ecs::RenderableComponent>();
			auto& tran = entity.getComponent<ecs::TransformComponent>();

			if (entity.hasComponent<ecs::ColorComponent>()) {
				auto& color = entity.getComponent<ecs::ColorComponent>();

				m_transformsColor.push_back(tran.transform);
				submitVerticesIndices(renderable, m_verticesColor, m_indicesColor, m_indicesMaxColor, m_counterColor, m_stride);

				m_samplersColors.push_back(color);
				m_counterColor++;
			}/*
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
			*/
			GRAPHICS_TRACE("RENDERERENTITY: submitted Entity!");
		}

		void RendererEntity::submitVerticesIndices(ecs::RenderableComponent& ren, std::vector<float>& vertices, 
			std::vector<uint32_t>& indices, uint32_t& indicesmax, int32_t& counter, uint32_t& stride)
		{
			std::vector<uint32_t> copy = ren.indices;

			ShapeManipulator::extendShapeID(ren.vertices, stride, (float)counter);
			ShapeManipulator::extendIndices(copy, indicesmax);

			vertices.insert(vertices.end(), ren.vertices.begin(), ren.vertices.end());
			indices.insert(indices.end(), copy.begin(), copy.end());

			indicesmax += ren.vertices.size() / stride;
		
			GRAPHICS_TRACE("RENDERERENTITY: submitted renderable component!");
		}

		void RendererEntity::update() {
			if (!m_verticesColor.empty()) {
				draw(m_verticesColor, m_indicesColor, m_transformsColor, m_samplersColors, m_shaderColor);
			}
			/*
			if (!m_verticesTexture2D.empty()) {
				draw(m_verticesTexture2D, m_indicesTexture2D, m_transformsTexture2D, m_samplersTexture2D, m_shaderTexture2D);
			}

			if (!m_verticesCubemap.empty()) {
				draw(m_verticesCubemap, m_indicesCubemap, m_transformsCubemap, m_samplersCubemap, m_shaderCubemap);
			}
			*/
			GRAPHICS_INFO("RENDERERENTITY: Draw calls finished for this scene!");
		}

		void RendererEntity::clear() {
			if (!m_lastSizeSet)
				return;

			m_verticesColor.clear();
			m_indicesColor.clear();
			m_transformsColor.clear();
			m_samplersColors.clear();
			m_counterColor = 0;
			m_indicesMaxColor = 0;
			/*
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
			*/
			GRAPHICS_TRACE("RENDERERENTITY: called clear method!");
		}

		void RendererEntity::draw(const std::vector<float>& vertices, const std::vector<uint32_t>& indices,
			const std::vector<maths::mat4>& transforms, const std::vector<maths::vec3>& samplers, ShaderOpenGL& shader)
		{
			GRAPHICS_TRACE("RENDERER: is preparing to draw!");

			s_stats.verticesCount += vertices.size();
			s_stats.indicesCount += indices.size();

			{ // SEND ALL DATA TO SHADERS
				shader.bind();

				passLightToShader(shader);
				passCameraToShader(shader);

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

			GRAPHICS_INFO("RENDERER: has drawn the scene!");
			GRAPHICS_INFO("Draw Call: " + std::to_string(s_stats.drawCallsCount));
		}

		void RendererEntity::draw(const std::vector<float>& vertices, const std::vector<uint32_t>& indices,
			const std::vector<maths::mat4>& transforms, const std::vector<int32_t>& samplers, ShaderOpenGL& shader)
		{
			GRAPHICS_TRACE("RENDERER: is preparing to draw!");

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

			GRAPHICS_INFO("RENDERER: has drawn the scene!");
			GRAPHICS_INFO("Draw Call: " + std::to_string(s_stats.drawCallsCount));
		}

		void RendererEntity::passLightToShader(ShaderOpenGL& shader) {
			MAR_CORE_ASSERT(m_lightPositions.size() == m_lightComponents.size(), "Light positions are not equal to light components!");
			
			using namespace ShaderUniforms;

			for (size_t i = 0; i < m_lightPositions.size(); i++) {
				shader.setUniformVector3(u_material[i].lightPos, m_lightPositions[i]);

				shader.setUniformVector3(u_material[i].ambient, m_lightComponents[i].ambient);
				shader.setUniformVector3(u_material[i].diffuse, m_lightComponents[i].diffuse);
				shader.setUniformVector3(u_material[i].specular, m_lightComponents[i].specular);

				shader.setUniform1f(u_material[i].shininess, m_lightComponents[i].shininess);

				shader.setUniform1f(u_material[i].constant, m_lightComponents[i].constant);
				shader.setUniform1f(u_material[i].linear, m_lightComponents[i].linear);
				shader.setUniform1f(u_material[i].quadratic, m_lightComponents[i].quadratic);
			}

			shader.setUniform1i(u_materialSize, m_lightPositions.size());

			GRAPHICS_TRACE("RENDERERENTITY: passed light to shader!");
		}

		void RendererEntity::passCameraToShader(ShaderOpenGL& shader) {
			shader.setUniformVector3("u_CameraPos", m_cameraCenter);
			shader.setUniformMat4f("u_Model", m_cameraModel);
			shader.setUniformMat4f("u_MVP", m_cameraMVP);

			GRAPHICS_TRACE("RENDERERENTITY: passed camera to shader (by values)!");
		}

		RendererStatistics& RendererEntity::getStatistics() { 
			return s_stats; 
		}

		void RendererEntity::clearStatistics() {
			s_stats.resetStatistics();

			GRAPHICS_INFO("RENDERERENTITY: reseting renderer statistics!");
		}

		void RendererEntity::updateTransforms(ecs::Scene* scene) {
			m_transformsColor.clear();
			//m_transformsTexture2D.clear();
			//m_transformsCubemap.clear();

			for (auto& entity : scene->entities) {
				if (entity.hasComponent<ecs::ColorComponent>())
					m_transformsColor.push_back(entity.getComponent<ecs::TransformComponent>());

				/*
				if (entity.hasComponent<ecs::Texture2DComponent>())
					m_transformsTexture2D.push_back(entity.getComponent<ecs::TransformComponent>());

				if (entity.hasComponent<ecs::TextureCubemapComponent>())
					m_transformsCubemap.push_back(entity.getComponent<ecs::TransformComponent>());
				*/
			}

			GRAPHICS_TRACE("RENDERINGENTITY: updating transforms");
		}

		void RendererEntity::updateColors(ecs::Scene* scene) {
			m_samplersColors.clear();

			for (auto& entity : scene->entities)
				if (entity.hasComponent<ecs::ColorComponent>())
					m_samplersColors.push_back(entity.getComponent<ecs::ColorComponent>());
			
			GRAPHICS_TRACE("RENDERERENTITY: updating colors");
		}

		void RendererEntity::updateLight(ecs::Scene* scene) {
			m_lightPositions.clear();
			m_lightComponents.clear();

			for (auto& entity : scene->entities) {
				if (entity.hasComponent<ecs::LightComponent>()) {
					m_lightComponents.push_back(entity.getComponent<ecs::LightComponent>());
					m_lightPositions.push_back(entity.getComponent<ecs::TransformComponent>().center);
				}
			}
			
			GRAPHICS_TRACE("RENDERERENTITY: updating light");
		}

		void RendererEntity::updateCamera(maths::mat4& projection, maths::mat4& view, maths::mat4& model, maths::vec3& position) {
			m_cameraModel = model;
			m_cameraMVP = projection * view * model;
			m_cameraCenter = position;

			GRAPHICS_TRACE("RENDERERENTITY: updated camera!");
		}


} }
