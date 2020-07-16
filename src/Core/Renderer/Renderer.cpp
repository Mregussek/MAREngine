/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Renderer.h"
#include "../../Debug/Log.h"

namespace mar {
	namespace graphics {

		RendererStatistics Renderer::s_stats;

		void Renderer::create() {
			MAR_CORE_TRACE("RENDERER: Trying to create renderer!");

			if (!m_initialized) {
				m_vbo = storage::factory->createVertexBuffer();
				m_layout = storage::factory->createVertexBufferLayout();
				m_vao = storage::factory->createVertexArray();
				m_ebo = storage::factory->createElementBuffer();

				MAR_CORE_INFO("RENDERER: created!");
			}
			else {
				MAR_CORE_ERROR("RENDERER: is already initialized!");
			}
		}

		void Renderer::close() {
			MAR_CORE_TRACE("RENDERER: Trying to close renderer!");

			if (m_initialized) {
				m_mainShader->shutdown();
				m_vao->closeArrayBuffer();
				m_vbo->close();
				m_ebo->close();

				m_mainShader.reset();
				m_vao.reset();
				m_vbo.reset();
				m_ebo.reset();
				m_layout.reset();

				m_initialized = false;

				MAR_CORE_INFO("RENDERER: closed!");
			}
			else {
				MAR_CORE_ERROR("RENDERER: is not initialized!");
			}
		}

		void Renderer::initialize(const std::vector<uint32_t>& layout, const ShaderType type) {
			MAR_CORE_TRACE("RENDERER: Trying to initialize renderer");

			if (!m_initialized) {
				m_mainShader = storage::factory->createShader(type);
				m_mainShader->initialize(type);

				for (size_t i = 0; i < layout.size(); i++)
					m_layout->push(layout[i], PushBuffer::PUSH_FLOAT);

				m_ebo->initializeElement(constants::maxIndexCount);
				m_vao->initializeArrayBuffer();
				m_vbo->initializeVertex(constants::maxVertexCount);
				m_vao->addBuffer(m_layout);

				m_initialized = true;

				MAR_CORE_INFO("RENDERER: properly initialized!");
			}
			else {
				MAR_CORE_ERROR("RENDERER: is already initialized!");
			}
		}

		void Renderer::draw(Mesh* mesh) {
			MAR_CORE_TRACE("RENDERER: is preparing to draw!");

			mesh->update();

			s_stats.verticesCount += mesh->getVertices().size();
			s_stats.indicesCount += mesh->getIndices().size();
			s_stats.shapesCount += mesh->getShapesCount();

			m_mainShader->bind();
			m_mainShader->setUniformSampler("u_Texture", mesh->getSamplers());
			updateMeshData(mesh);
			updateCameraData();
			updateGUIData();
			updateLightData(&mesh->getLight());

			m_vao->bind();

			m_vbo->bind();
			m_vbo->updateDynamically(mesh->getVertices());

			m_ebo->bind();
			m_ebo->updateDynamically(mesh->getIndices());

			MAR_CORE_GL_FUNC( glDrawElements(GL_TRIANGLES, mesh->getIndices().size(), GL_UNSIGNED_INT, nullptr) );

			m_vbo->resetBuffer();
			m_vbo->unbind();

			m_ebo->resetBuffer();
			m_ebo->unbind();

			m_vao->unbind();

			s_stats.drawCallsCount += 1;
			s_stats.trianglesCount = s_stats.indicesCount / 3;

			MAR_CORE_INFO("RENDERER: has drawn the scene!");
			MAR_CORE_INFO("Draw Call: " + std::to_string(s_stats.drawCallsCount));
		}

		void Renderer::updateMeshData(Mesh* mesh) {
			MAR_CORE_TRACE("RENDERER: updating mesh data");

			if (storage::usegui) {
				mesh->clearMatrices();

				for (uint32_t i = 0; i < mesh->getShapesCount(); i++) 
					mesh->pushMatrices(mesh->getShape(i));
			}

			m_mainShader->setUniformVectorMat4("u_SeperateTranslate", mesh->getTranslationMatrices());
			m_mainShader->setUniformVectorMat4("u_SeperateRotation", mesh->getRotationMatrices());
			m_mainShader->setUniformVectorMat4("u_SeperateScale", mesh->getScaleMatrices());

			m_mainShader->setUniformVectorVec3("u_SeparateColor", mesh->getColors());
		}

		void Renderer::updateGUIData() {
			MAR_CORE_TRACE("RENDERER: updating gui data");

			if (!storage::usegui)
				return;

			m_mainShader->setUniform4fv("u_GUISceneColor", editor::GUI::getGUIData().colors);

			m_mainShader->setUniformMat4f("u_GUISceneTranslation", editor::GUI::getGUIData().translate);
			m_mainShader->setUniformMat4f("u_GUISceneRotation", editor::GUI::getGUIData().rotation);
		}

		void Renderer::updateCameraData() {
			MAR_CORE_TRACE("RENDERER: updating camera data");

			m_mainShader->setUniformMat4f("u_MVP", Camera::getCameraData().mvp);
			m_mainShader->setUniformMat4f("u_Model", Camera::getCameraData().model);

			m_mainShader->setUniformVector3("u_CameraPos", Camera::getCameraData().position);
		}

		void Renderer::updateLightData(Light* light) {
			MAR_CORE_TRACE("RENDERER: updating light data");

			m_mainShader->setUniformVector3("u_material.lightPos", light->getPosition());

			m_mainShader->setUniformVector3("u_material.ambient", light->getAmbient());
			m_mainShader->setUniformVector3("u_material.diffuse", light->getDiffuse());
			m_mainShader->setUniformVector3("u_material.specular", light->getSpecular());

			m_mainShader->setUniformVector3("u_material.ambientStrength", light->getAmbientStrength());
			m_mainShader->setUniformVector3("u_material.diffuseStrength", light->geDiffuseStrength());
			m_mainShader->setUniformVector3("u_material.specularStrength", light->geSpecularStrength());

			m_mainShader->setUniform1f("u_material.shininess", light->getShininess());

			m_mainShader->setUniform1f("u_material.constant", light->getConstant());
			m_mainShader->setUniform1f("u_material.linear", light->getLinear());
			m_mainShader->setUniform1f("u_material.quadratic", light->getQuadratic());
		}


} }