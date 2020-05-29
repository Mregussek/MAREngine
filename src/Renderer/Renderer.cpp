/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Renderer.h"

namespace mar {

	void Renderer::createRenderer(const std::shared_ptr<RendererFactory>& factory) {
		if (!m_initialized) {
			m_vbo = factory->createVertexBuffer();
			m_layout = factory->createVertexBufferLayout();
			m_vao = factory->createVertexArray();
			m_ebo = factory->createElementBuffer();
			m_texture = factory->createTexture();
			m_mainShader = factory->createShader();

			m_stats = RendererStatistics();
			m_pushedLayout = false;
			_nextTextureID = 1.0f;
		}
		else {
			std::cerr << "Renderer is already initialized!" << std::endl;
		}
	}

	void Renderer::closeRenderer() {
		if (m_initialized) {
			m_mainShader->shutdown();
			m_texture->shutdown();
			m_vao->closeArrayBuffer();
			m_vbo->close();
			m_ebo->close();
		}
		else {
			std::cerr << "Renderer is not initialized!" << std::endl;
		}
	}

	void Renderer::initialize(Mesh* mesh) {
		// When we will use Engine as a real AR app we won't to see a manageable GUI
		if (m_isGUIconnected)
			m_mainShader->initialize(ShaderType::DEFAULT);
		else
			m_mainShader->initialize(ShaderType::WITHOUT_GUI);

		if (!m_pushedLayout) {
			std::vector<unsigned int> layout = mesh->getLayout();

			for (size_t i = 0; i < mesh->getLayoutSize(); i++)
				m_layout->push(layout[i], PushBuffer::PUSH_FLOAT);

			m_pushedLayout = true;
		}

		m_vao->initializeArrayBuffer();
		m_vbo->initializeVertex(constants::maxVertexCount);
		m_ebo->initializeElement(constants::maxIndexCount);

		m_vao->addBuffer(m_layout);

		for (unsigned int i = 0; i < mesh->getSamplersSize(); i++)
			m_texture->bind(mesh->getSamplerID(i), m_texture->getID(i));

		m_mainShader->bind();

		m_initialized = true;
	}

	void Renderer::loadScene(Mesh* mesh, Scene* scene) {
		for (unsigned int i = 0; i < scene->getShapesNumber(); i++)
			addShape(mesh, scene->getShape(i), scene->getCenter(i), scene->getAngle(i), scene->getTexture(i));
	}

	void Renderer::addShape(Mesh* mesh, std::shared_ptr<Shape>& shape, const glm::vec3& center, const glm::vec3& angle, std::string texturePath) {
		if (texturePath == "empty") {
			ShapeManipulator::extendTextureID(shape, 0.0f);
		}

		else {
			ShapeManipulator::extendTextureID(shape, _nextTextureID);
			_nextTextureID++;

			m_texture->loadTexture(texturePath);
		}

		_names.push_back(shape->getName());

		mesh->pushShape(shape);
		mesh->pushMatrices(center, angle);
	}

	void Renderer::draw(Mesh* mesh) {
		bind();

		m_stats.resetStatistics();

		mesh->clearBuffers();
		mesh->update();

		m_vbo->updateDynamically(mesh->getVertices());
		m_ebo->updateDynamically(mesh->getIndices());

		m_stats._countOfVertices = mesh->getVerticesSize();
		m_stats._countOfIndices = mesh->getIndicesSize();
		m_stats._countOfShapes = mesh->getShapesCount();

		if (!m_isGUIconnected) {
			m_mainShader->setUniformVectorMat4("u_SeperateTranslate", mesh->getTranslationMatrices());
			m_mainShader->setUniformVectorMat4("u_SeperateRotation",mesh->getRotationMatrices());
		}

		m_mainShader->setUniformVector3("u_LightPos", mesh->getLightPosition());
		m_mainShader->setUniformSampler2D("u_Texture", mesh->getSamplers());

		glDrawElements(GL_TRIANGLES, mesh->getIndicesSize(), GL_UNSIGNED_INT, nullptr);

		m_stats._countOfDrawCalls = 1;
		m_stats._countOfTriangles = m_stats._countOfIndices / 3;

		for (unsigned int i = 0; i < mesh->getSamplersSize(); i++)
			m_texture->bind(mesh->getSamplerID(i), m_texture->getID(i));
	}

	void Renderer::connectGUI() {
		m_isGUIconnected = true; 
	}

	void Renderer::disconnectGUI() { 
		m_isGUIconnected = false; 
	}

	void Renderer::updateGUIData(Mesh* mesh, const GUIData* guidata) {
		if (!m_isGUIconnected) {
			std::cerr << "GUI is not connected!" << std::endl;
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
}