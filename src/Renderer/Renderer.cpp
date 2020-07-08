/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Renderer.h"
#include "../Debug/Log.h"

namespace mar {
	namespace graphics {

		RendererStatistics Renderer::s_stats;

		void Renderer::create() {
			if (!m_initialized) {
				m_vbo = storage::factory->createVertexBuffer();
				m_layout = storage::factory->createVertexBufferLayout();
				m_vao = storage::factory->createVertexArray();
				m_ebo = storage::factory->createElementBuffer();

				s_stats = RendererStatistics();

				MAR_CORE_INFO("Renderer properly created!");
			}
			else {
				MAR_CORE_ERROR("Renderer is already initialized!");
			}
		}

		void Renderer::close() {
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

				MAR_CORE_INFO("Renderer closed!");
			}
			else {
				MAR_CORE_ERROR("Renderer is not initialized!");
			}
		}

		void Renderer::initialize(const std::vector<unsigned int>& layout, const ShaderType type) {
			if (!m_initialized) {
				m_mainShader = storage::factory->createShader(type);
				m_mainShader->initialize(type);

				for (size_t i = 0; i < layout.size(); i++)
					m_layout->push(layout[i], PushBuffer::PUSH_FLOAT);

				m_ebo->initializeElement(constants::maxIndexCount);
				m_ebo->unbind();

				m_vao->initializeArrayBuffer();
				m_vbo->initializeVertex(constants::maxVertexCount);
				m_vao->addBuffer(m_layout);
				m_vbo->unbind();
				m_vao->unbind();

				m_initialized = true;

				MAR_CORE_INFO("Renderer properly initialized!");
			}
			else {
				MAR_CORE_ERROR("Renderer is already initialized!");
			}
		}

		void Renderer::draw(Mesh* mesh) {
			mesh->clearBuffers();
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
		}

		void Renderer::updateMeshData(Mesh* mesh) {
			if (storage::usegui) {
				mesh->clearMatrices();

				for (unsigned int i = 0; i < mesh->getShapesCount(); i++) {
					mesh->pushMatrices(mesh->getShape(i)->getCenter(), mesh->getShape(i)->getAngle(), 
						mesh->getShape(i)->getScale());
				}
			}

			m_mainShader->setUniformVectorMat4("u_SeperateTranslate", mesh->getTranslationMatrices());
			m_mainShader->setUniformVectorMat4("u_SeperateRotation", mesh->getRotationMatrices());
			m_mainShader->setUniformVectorMat4("u_SeperateScale", mesh->getScaleMatrices());
			m_mainShader->setUniformVectorVec3("u_SeparateColor", mesh->getColors());
		}

		void Renderer::updateGUIData() {
			if (storage::usegui) {
				m_mainShader->setUniform4fv("u_GUISceneColor", gui::GUI::getGUIData().colors);
				m_mainShader->setUniformMat4f("u_GUISceneTranslation", gui::GUI::getGUIData().translate);
				m_mainShader->setUniformMat4f("u_GUISceneRotation", gui::GUI::getGUIData().rotation);
			}
		}

		void Renderer::updateCameraData() {
			m_mainShader->setUniformMat4f("u_MVP", Camera::getCameraData().mvp);
			m_mainShader->setUniformMat4f("u_Model", Camera::getCameraData().model);

			m_mainShader->setUniformVector3("u_CameraPos", Camera::getCameraData().position);
		}

		void Renderer::updateLightData(Light* light) {
			light->setPosition({
				0.5f + 0.5f * sin(glfwGetTime()),
				0.2f + 0.2f * sin(glfwGetTime()),
				7.0f + 5.0f * cos(glfwGetTime())
			});

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