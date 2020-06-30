/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Renderer.h"
#include "../Debug/Log.h"

namespace mar {
	namespace graphics {

		RendererStatistics Renderer::s_stats;

		void Renderer::createRenderer(const Ref<RendererFactory>& factory, const bool& usegui) {
			if (!m_initialized) {
				m_vbo = factory->createVertexBuffer();
				m_layout = factory->createVertexBufferLayout();
				m_vao = factory->createVertexArray();
				m_ebo = factory->createElementBuffer();
				m_mainShader = factory->createShader();

				s_stats = RendererStatistics();
				m_useGUI = usegui;

				MAR_CORE_INFO("Renderer properly created!");
			}
			else {
				MAR_CORE_ERROR("Renderer is already initialized!");
			}
		}

		void Renderer::closeRenderer() {
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
				m_mainShader->initialize(type);

				for (size_t i = 0; i < layout.size(); i++)
					m_layout->push(layout[i], PushBuffer::PUSH_FLOAT);

				m_vao->initializeArrayBuffer();
				m_vbo->initializeVertex(constants::maxVertexCount);
				m_ebo->initializeElement(constants::maxIndexCount);

				m_vao->addBuffer(m_layout);

				m_initialized = true;

				MAR_CORE_INFO("Renderer properly initialized!");
			}
			else {
				MAR_CORE_ERROR("Renderer is already initialized!");
			}
		}

		void Renderer::draw(Mesh* mesh) {
			bind();

			mesh->clearBuffers();
			mesh->update();

			updateMeshData(mesh);
			updateCameraData();
			updateGUIData();
			updateLightData(&mesh->getLight());

			m_vbo->updateDynamically(mesh->getVertices());
			m_ebo->updateDynamically(mesh->getIndices());

			s_stats.verticesCount += mesh->getVertices().size();
			s_stats.indicesCount += mesh->getIndices().size();
			s_stats.shapesCount += mesh->getShapesCount();

			m_mainShader->setUniformSampler("u_Texture", mesh->getSamplers());

			glDrawElements(GL_TRIANGLES, mesh->getIndices().size(), GL_UNSIGNED_INT, nullptr);

			s_stats.drawCallsCount += 1;
			s_stats.trianglesCount = s_stats.indicesCount / 3;

			unbind();
		}

		void Renderer::updateMeshData(Mesh* mesh) {
			if (m_useGUI) {
				mesh->clearMatrices();

				for (unsigned int i = 0; i < mesh->getShapesCount(); i++) {
					mesh->pushMatrices(mesh->getShape(i)->getCenter(), mesh->getShape(i)->getAngle());
				}
			}

			m_mainShader->setUniformVectorMat4("u_SeperateTranslate", mesh->getTranslationMatrices());
			m_mainShader->setUniformVectorMat4("u_SeperateRotation", mesh->getRotationMatrices());
			m_mainShader->setUniformVectorVec3("u_SeparateColor", mesh->getColors());
		}

		void Renderer::updateGUIData() {
			if (m_useGUI) {
				m_mainShader->setUniform4fv("u_GUISceneColor", m_guiData->colors);
				m_mainShader->setUniformMat4f("u_GUISceneTranslation", m_guiData->translate);
				m_mainShader->setUniformMat4f("u_GUISceneRotation", m_guiData->rotation);
			}
		}

		void Renderer::updateCameraData() {
			m_mainShader->setUniformMat4f("u_Projection", m_cameraData->projection);
			m_mainShader->setUniformMat4f("u_View", m_cameraData->view);
			m_mainShader->setUniformMat4f("u_Model", m_cameraData->model);

			m_mainShader->setUniformVector3("u_CameraPos", m_cameraData->position);
		}

		void Renderer::updateLightData(Light* light) {
			light->setPosition({
				0.5f + 0.5f * sin(glfwGetTime()),
				light->getPosition().y,
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

		void Renderer::bind() {
			m_mainShader->bind();
			m_vao->bind();
			m_vbo->bind();
			m_ebo->bind();
		}

		void Renderer::unbind() {
			m_mainShader->unbind();
			m_vao->unbind();
			m_vbo->unbind();
			m_ebo->unbind();
		}

		void Renderer::setReferences(const gui::GUIData* guidata, const CameraData* cameradata) {
			MAR_CORE_TRACE("Setting references for Renderer (GUIData and CameraData)!");

			m_guiData = guidata;
			m_cameraData = cameradata;
		}

		void Renderer::setReferences(const CameraData* cameradata) {
			MAR_CORE_TRACE("Setting references for Renderer (CameraData)!");

			m_cameraData = cameradata;
		}


} }