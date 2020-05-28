/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Renderer.h"

namespace mar {

	void Renderer::createRenderer(const std::shared_ptr<RendererFactory>& factory) {
		if (!_initialized) {
			_vbo = factory->createVertexBuffer();
			_lay = factory->createVertexBufferLayout();
			_vao = factory->createVertexArray();
			_ebo = factory->createElementBuffer();
			_texture = factory->createTexture();
			_mainShader = factory->createShader();

			_stats = RendererStatistics();
			_pushedLayout = false;
			_nextTextureID = 1.0f;
		}
		else {
			std::cerr << "Renderer is already initialized!" << std::endl;
		}
	}

	void Renderer::closeRenderer() {
		if (_initialized) {
			_mainShader->shutdown();
			_texture->shutdown();
			_vao->closeArrayBuffer();
			_vbo->close();
			_ebo->close();
		}
		else {
			std::cerr << "Renderer is not initialized!" << std::endl;
		}
	}

	void Renderer::initialize(Mesh* mesh) {
		// When we will use Engine as a real AR app we won't to see a manageable GUI
		if (_isGUIconnected)
			_mainShader->initialize(ShaderType::DEFAULT);
		else
			_mainShader->initialize(ShaderType::WITHOUT_GUI);

		if (!_pushedLayout) {
			std::vector<unsigned int> layout = mesh->getLayout();

			for (size_t i = 0; i < mesh->getLayoutSize(); i++)
				_lay->push(layout[i], PushBuffer::PUSH_FLOAT);

			_pushedLayout = true;
		}

		_vao->initializeArrayBuffer();
		_vbo->initializeVertex(constants::maxVertexCount);
		_ebo->initializeElement(constants::maxIndexCount);

		_vao->addBuffer(_lay);

		for (unsigned int i = 0; i < mesh->getSamplersSize(); i++)
			_texture->bind(mesh->getSamplerID(i), _texture->getID(i));

		_mainShader->bind();

		_initialized = true;
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

			_texture->loadTexture(texturePath);
		}

		_names.push_back(shape->getName());

		mesh->pushShape(shape);
		mesh->pushMatrices(center, angle);
	}

	void Renderer::draw(Mesh* mesh) {
		bind();

		_stats.resetStatistics();

		mesh->clearBuffers();
		mesh->update();

		_vbo->updateDynamically(mesh->getVertices());
		_ebo->updateDynamically(mesh->getIndices());

		_stats._countOfVertices = mesh->getVerticesSize();
		_stats._countOfIndices = mesh->getIndicesSize();
		_stats._countOfShapes = mesh->getShapesCount();

		if (!_isGUIconnected) {
			_mainShader->setUniformVectorMat4("u_SeperateTranslate", mesh->getTranslationMatrices());
			_mainShader->setUniformVectorMat4("u_SeperateRotation",mesh->getRotationMatrices());
		}

		_mainShader->setUniformVector3("u_LightPos", mesh->getLightPosition());
		_mainShader->setUniformSampler2D("u_Texture", mesh->getSamplers());

		glDrawElements(GL_TRIANGLES, mesh->getIndicesSize(), GL_UNSIGNED_INT, nullptr);

		_stats._countOfDrawCalls = 1;
		_stats._countOfTriangles = _stats._countOfIndices / 3;

		for (unsigned int i = 0; i < mesh->getSamplersSize(); i++)
			_texture->bind(mesh->getSamplerID(i), _texture->getID(i));
	}

	void Renderer::connectGUI() {
		_isGUIconnected = true; 
	}

	void Renderer::disconnectGUI() { 
		_isGUIconnected = false; 
	}

	void Renderer::updateGUIData(Mesh* mesh, const GUIData* guidata) {
		if (!_isGUIconnected) {
			std::cerr << "GUI is not connected!" << std::endl;
			return;
		}

		mesh->clearMatrices();

		for (unsigned int i = 0; i < mesh->getShapesCount(); i++) 
			mesh->pushMatrices(guidata->centers[i], guidata->angles[i]);

		_mainShader->setUniformVectorMat4("u_GUISeperateTranslate", mesh->getTranslationMatrices());
		_mainShader->setUniformVectorMat4("u_GUISeperateRotation", mesh->getRotationMatrices());

		_mainShader->setUniform4fv("u_GUISceneColor", guidata->colors);
		_mainShader->setUniformMat4f("u_GUISceneTranslation", guidata->translate);
		_mainShader->setUniformMat4f("u_GUISceneRotation", guidata->rotation);
	}

	void Renderer::updateCameraData(const CameraData* cameradata) {
		_mainShader->setUniformMat4f("u_Projection", cameradata->projection);
		_mainShader->setUniformMat4f("u_View", cameradata->view);
		_mainShader->setUniformMat4f("u_Model", cameradata->model);

		_mainShader->setUniformVector3("u_CameraPos", cameradata->position);
	}

	void Renderer::bind() {
		_mainShader->bind();
		_vao->bind();
		_vbo->bind();
		_ebo->bind();
	}

	void Renderer::unbind() {
		_mainShader->unbind();
		_vao->unbind();
		_vbo->unbind();
		_ebo->unbind();
	}
}