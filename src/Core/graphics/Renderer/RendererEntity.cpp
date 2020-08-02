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
			m_shaderTextures2D.shutdown();
			m_shaderCubemaps.shutdown();

			GRAPHICS_INFO("RENDERERENTITY: closed!");
		}

		void RendererEntity::submit(ecs::Scene* scene) {
			if (m_lastSize == scene->entities.size()) {
				if (scene->updatedBuffers || scene->updatedTextures2D)
					goto resubmit_all;

				if (scene->updatedTransforms)
					updateTransforms(scene);

				scene->updatedTransforms = false;

				if (scene->updatedColors)
					updateColors(scene);

				scene->updatedColors = false;

				if (scene->updatedLight)
					updateLight(scene);
				
				scene->updatedLight = false;

				updateCamera(scene);

				m_lastSizeSet = false;
				return;
			}
				
		resubmit_all:

			scene->updatedBuffers = false;
			scene->updatedTextures2D = false;

			m_lastSize = scene->entities.size();
			m_lastSizeSet = true;

			clear();

			updateLight(scene);
			updateCamera(scene);

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

				m_storageColor.transforms.push_back(tran.transform);
				submitVerticesIndices(renderable, m_storageColor.vertices, m_storageColor.indices, m_storageColor.indicesMax, m_storageColor.counter, m_stride);

				m_storageColor.samplers.push_back(color);
				m_storageColor.counter++;
			}
			
			if (entity.hasComponent<ecs::Texture2DComponent>()) {
				auto& texture = entity.getComponent<ecs::Texture2DComponent>();

				m_textures.push_back(texture.texture);

				float id = m_texture.loadTexture(texture.texture.c_str());

				m_storageTexture2D.transforms.push_back(tran.transform);
				submitVerticesIndices(renderable, m_storageTexture2D.vertices, m_storageTexture2D.indices, m_storageTexture2D.indicesMax, m_storageTexture2D.counter, m_stride);

				m_storageTexture2D.samplers.push_back(m_storageTexture2D.counter);
				m_storageTexture2D.counter++;
			}

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
			if (!m_storageColor.vertices.empty()) {
				draw(m_storageColor.vertices, m_storageColor.indices, m_storageColor.transforms, m_storageColor.samplers, m_shaderColor);
			}

			if (!m_storageTexture2D.vertices.empty()) {
				draw(m_storageTexture2D.vertices, m_storageTexture2D.indices, m_storageTexture2D.transforms, m_storageTexture2D.samplers, m_shaderTextures2D);
			}

			GRAPHICS_INFO("RENDERERENTITY: Draw calls finished for this scene!");
		}

		void RendererEntity::clear() {
			if (!m_lastSizeSet)
				return;

			m_storageColor.vertices.clear();
			m_storageColor.indices.clear();
			m_storageColor.transforms.clear();
			m_storageColor.samplers.clear();
			m_storageColor.counter = 0;
			m_storageColor.indicesMax = 0;

			m_textures.clear();
			m_storageTexture2D.vertices.clear();
			m_storageTexture2D.indices.clear();
			m_storageTexture2D.transforms.clear();
			m_storageTexture2D.samplers.clear();
			m_storageTexture2D.counter = 0;
			m_storageTexture2D.indicesMax = 0;

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

			{ // BIND TEXTURES
				for (int32_t i = 0; i < samplers.size(); i++) 
					m_texture.bind(GL_TEXTURE_2D, samplers[i], m_texture.getTexture(m_textures[i]));
			}

			{ // SEND ALL DATA TO SHADERS
				shader.bind();

				passLightToShader(shader);
				passCameraToShader(shader);

				shader.setUniformVectorMat4("u_SeparateTransform", transforms);
				shader.setUniformSampler("u_SeparateColor", samplers);
			}

			{ // BIND ALL NEEDED BUFFERS
				m_vao.bind();

				m_vbo.bind();
				m_vbo.updateDynamically(vertices);

				m_ebo.bind();
				m_ebo.update(indices);
			}

			MAR_CORE_GL_FUNC(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr));

			{ // UNBIND ALREADY DRAWN BUFFERS
				m_vbo.resetBuffer();
				m_vbo.unbind();

				m_ebo.resetBuffer();
				m_ebo.unbind();

				m_vao.unbind();
			}

			s_stats.verticesCount += vertices.size();
			s_stats.indicesCount += indices.size();

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
			m_storageColor.transforms.clear();
			m_storageTexture2D.transforms.clear();

			for (auto& entity : scene->entities) {
				if (entity.hasComponent<ecs::ColorComponent>())
					m_storageColor.transforms.push_back(entity.getComponent<ecs::TransformComponent>());
			
				if (entity.hasComponent<ecs::Texture2DComponent>())
					m_storageTexture2D.transforms.push_back(entity.getComponent<ecs::TransformComponent>());
			}

			GRAPHICS_TRACE("RENDERINGENTITY: updating transforms");
		}

		void RendererEntity::updateColors(ecs::Scene* scene) {
			m_storageColor.samplers.clear();

			for (auto& entity : scene->entities)
				if (entity.hasComponent<ecs::ColorComponent>())
					m_storageColor.samplers.push_back(entity.getComponent<ecs::ColorComponent>());
			
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

		void RendererEntity::updateCamera(ecs::Scene* scene) {
			if (scene->useEditorCamera) {
				auto& camdata = editor::Camera::getCameraData();

				m_cameraModel = camdata.model;
				m_cameraMVP = camdata.mvp;
				m_cameraCenter = camdata.position;

				GRAPHICS_TRACE("RENDERERENTITY: updated camera (using Editor Camera)!");

				return;
			}

			auto& camdata = scene->scene_camera;

			m_cameraModel = camdata.model;
			m_cameraMVP = camdata.projection * camdata.view * camdata.model;
			m_cameraCenter = camdata.position;

			GRAPHICS_TRACE("RENDERERENTITY: updated camera - used SceneCamera!");
		}


} }
