/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Renderer.h"

namespace mar {
	namespace graphics {


		void Renderer::createRenderer(const std::shared_ptr<RendererFactory>& factory) {
			if (!m_initialized) {
				m_vbo = factory->createVertexBuffer();
				m_layout = factory->createVertexBufferLayout();
				m_vao = factory->createVertexArray();
				m_ebo = factory->createElementBuffer();
				m_mainShader = factory->createShader();

				m_stats = RendererStatistics();
			}
			else {
				std::cerr << "Renderer is already initialized!" << std::endl;
			}
		}

		void Renderer::closeRenderer() {
			if (m_initialized) {
				m_mainShader->shutdown();
				m_vao->closeArrayBuffer();
				m_vbo->close();
				m_ebo->close();
			}
			else {
				std::cerr << "Renderer is not initialized!" << std::endl;
			}
		}

		void Renderer::initialize(const std::vector<unsigned int>& layout, const bool& useGUI) {
			if (!m_initialized) {
				m_useGUI = useGUI;

				if (m_useGUI)
					m_mainShader->initialize(ShaderType::DEFAULT);
				else
					m_mainShader->initialize(ShaderType::WITHOUT_GUI);

				for (size_t i = 0; i < layout.size(); i++)
					m_layout->push(layout[i], PushBuffer::PUSH_FLOAT);

				m_vao->initializeArrayBuffer();
				m_vbo->initializeVertex(constants::maxVertexCount);
				m_ebo->initializeElement(constants::maxIndexCount);

				m_vao->addBuffer(m_layout);

				m_mainShader->bind();

				m_initialized = true;
			}
		}

		void Renderer::draw(Mesh* mesh) {
			bind();

			m_stats.resetStatistics();

			mesh->clearBuffers();
			mesh->update();

			m_vbo->updateDynamically(mesh->getVertices());
			m_ebo->updateDynamically(mesh->getIndices());

			m_stats._countOfVertices += mesh->getVerticesSize();
			m_stats._countOfIndices += mesh->getIndicesSize();
			m_stats._countOfShapes += mesh->getShapesCount();

			m_mainShader->setUniformVector3("u_LightPos", mesh->getLightPosition());
			m_mainShader->setUniformSampler2D("u_Texture", mesh->getSamplers());

			glDrawElements(GL_TRIANGLES, mesh->getIndicesSize(), GL_UNSIGNED_INT, nullptr);

			m_stats._countOfDrawCalls += 1;
			m_stats._countOfTriangles = m_stats._countOfIndices / 3;
		}

		void Renderer::updateGUIData(Mesh* mesh, const gui::GUIData* guidata) {
			if (!m_useGUI) {
				m_mainShader->setUniformVectorMat4("u_SeperateTranslate", mesh->getTranslationMatrices());
				m_mainShader->setUniformVectorMat4("u_SeperateRotation", mesh->getRotationMatrices());
				
				return;
			}

			mesh->clearMatrices();

			for (unsigned int i = 0; i < mesh->getShapesCount(); i++)
				mesh->pushMatrices(guidata->centers[i], guidata->angles[i]);

			m_mainShader->setUniformVectorMat4("u_GUISeperateTranslate", mesh->getTranslationMatrices());
			m_mainShader->setUniformVectorMat4("u_GUISeperateRotation", mesh->getRotationMatrices());

			m_mainShader->setUniform4fv("u_GUISceneColor", guidata->colors);
			m_mainShader->setUniformMat4f("u_GUISceneTranslation", guidata->translate);
			m_mainShader->setUniformMat4f("u_GUISceneRotation", guidata->rotation);
		}

		void Renderer::updateCameraData(const CameraData* cameradata) {
			m_mainShader->setUniformMat4f("u_Projection", cameradata->projection);
			m_mainShader->setUniformMat4f("u_View", cameradata->view);
			m_mainShader->setUniformMat4f("u_Model", cameradata->model);

			m_mainShader->setUniformVector3("u_CameraPos", cameradata->position);
		}

		void Renderer::bind() {
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


} }