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

			_shapes = std::vector<std::shared_ptr<Shape>>();
			_stats = RendererStatistics();
			_pushedLayout = false;
			_maxValue = 0;
			_nextShapeID = 0.0f;
			_nextTextureID = 1.0f; // from 1.0f, cause 0.0f is reserved for default color
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
	
	void Renderer::initialize() {
		// When we will use Engine as a real AR app we won't to see a manageable GUI
		if(_isGUIconnected)
			_mainShader->initialize(ShaderType::DEFAULT);
		else
			_mainShader->initialize(ShaderType::WITHOUT_GUI);

		_vao->initializeArrayBuffer();
		_vbo->initializeVertex(constants::maxVertexCount);
		_ebo->initializeElement(constants::maxIndexCount);

		_vao->addBuffer(_lay);

		for (unsigned int i = 0; i < _samplers.size(); i++)
			_texture->bind((float)_samplers[i], _texture->getID(i));
			
		_mainShader->bind();

		_initialized = true;
	}

	void Renderer::loadScene(Scene* scene) {
		for (unsigned int i = 0; i < scene->getShapesNumber(); i++) {
			pushObject(scene->getShape(i), scene->getCenter(i), scene->getTexture(i));
			_translations.push_back(glm::translate(glm::mat4(1.0f), scene->getCenter(i)));
			_rotations.push_back(ShapeManipulator::getRotationMatrix(scene->getCenter(i), scene->getAngle(i)));
		}
	}

	///! TODO: When you:
	///! 1. Add new object to scene
	///! 2. Delete this object from scene
	///! 3. Just add new object to scene
	///! Program crashes because next texture id is being increased, and the number is not correct
	///! That's why we have segmentation fault. We have to investigate, why after loading scene we have 
	///! increased value
	///! Also look at TODO at popObject() method. There is associated problem!
	void Renderer::pushObject(std::shared_ptr<Shape>& shape, glm::vec3& position, std::string texturePath) {
		if (_shapes.size() == constants::maxObjectsInScene - 1) {
			std::cout << "Cannot push more objects!" << std::endl;
			return;
		}

		if (!_pushedLayout) {
			for (size_t i = 0; i < shape->getLayoutSize(); i++)
				_lay->push(shape->getLayout(i), PushBuffer::PUSH_FLOAT);

			_pushedLayout = true;
		}

		ShapeManipulator::extendShapeID(shape, _nextShapeID);
		_nextShapeID++;

		ShapeManipulator::changeIndicesFormat(shape, _maxValue);

		_maxValue += shape->getSizeofVertices() / shape->getStride();

		if (texturePath == "empty") {
			ShapeManipulator::extendTextureID(shape, 0.0f);
		}
		else {
			ShapeManipulator::extendTextureID(shape, _nextTextureID);
			_nextTextureID++;

			_texture->loadTexture(texturePath);

			_samplers.push_back((int)shape->getTextureID());
		}

		_shapes.emplace_back(shape);
	}

	///! TODO: When you:
	///! 1. Push a few objects to scene (for instance 5)
	///! 2. Delete object with index 0
	///! 3. and then again index 0 (This index was one, but because of vector reshape now is 0)
	///! program crashes, because of memory leak. We cannot use the same memory address for new object
	///! Maybe we should delete existing vector and copy its whole stuff to new one?
	///! Or use shapes as shared_ptr's, and with reset() method push new element and delete existing ones?
	void Renderer::popObject(const unsigned int& index) {
		if (_shapes[index]->getTextureID() != 0.0f) {
			_samplers.erase(_samplers.begin() + index);
			_texture->removeID(index);
		}
		
		_shapes[index].reset();
		_shapes.erase(_shapes.begin() + index);

		_vertices.clear();
		_indices.clear();
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

	///! TODO: Definitely need to work on this loop, when there is no memory, which we need there is still no draw
	///! We have one draw call, because we have this memory available, but when we need second draw call program crashes!
	void Renderer::updateFrame() {
		bind();

		_stats.resetStatistics();
		clearBuffer();

		// Main Loop - Batch Rendering
		for (unsigned int i = 0; i < _shapes.size(); i++) {
			if (_vertices.size() + _shapes[i]->getSizeofVertices() <= constants::maxVertexCount
							&& _indices.size() + _shapes[i]->getSizeofIndices() <= constants::maxIndexCount) {

				_vertices.insert(_vertices.end(), _shapes[i]->getVerticesBegin(), _shapes[i]->getVerticesEnd());
				_indices.insert(_indices.end(), _shapes[i]->getIndicesBegin(), _shapes[i]->getIndicesEnd());

				_stats._countOfVertices += _shapes[i]->getSizeofVertices();
				_stats._countOfIndices += _shapes[i]->getSizeofIndices();
				_stats._countOfShapes++;
			} 
			else {
				_vbo->updateDynamically(_vertices);
				_ebo->updateDynamically(_indices);

				draw();

				_stats._countOfDrawCalls++;

				clearBuffer();
			}
		}

		_vbo->updateDynamically(_vertices);
		_ebo->updateDynamically(_indices);
		draw();
		_stats._countOfDrawCalls++;
		_stats._countOfTriangles = _stats._countOfIndices / 3;

		for (unsigned int i = 0; i < _samplers.size(); i++) 
			_texture->bind((float)_samplers[i], _texture->getID(i));
	}

	void Renderer::draw() {
		// --- GUI UNIFORMS --- //
		if (!_isGUIconnected) {
			_mainShader->setUniformVectorMat4("u_SeperateTranslate", _translations);
			_mainShader->setUniformVectorMat4("u_SeperateRotation", _rotations);
		}

		// --- LIGHTS UNIFORMS --- //		
		_mainShader->setUniformVector3("u_LightPos", _lightPosition);

		// --- TEXTURE UNIFORMS --- //
		_mainShader->setUniformSampler2D("u_Texture", getSamplers());

		// --- MAIN DRAW CALL --- // 
		glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::clearBuffer() {
		_vertices.clear();
		_indices.clear();
	}

	void Renderer::guiPush(GUIPushType pushType, glm::vec3& position) {
		if (pushType == GUIPushType::PYRAMID)
			_addedDuringRuntime = std::make_shared<Pyramid>();
		else if (pushType == GUIPushType::CUBE)
			_addedDuringRuntime = std::make_shared<Cube>();
		else if (pushType == GUIPushType::SURFACE)
			_addedDuringRuntime = std::make_shared<Surface>();
		else if (pushType == GUIPushType::WALL)
			_addedDuringRuntime = std::make_shared<Wall>();

		pushObject(_addedDuringRuntime, position);
	}

	void Renderer::connectGUI() {
		_isGUIconnected = true; 
	}

	void Renderer::disconnectGUI() { 
		_isGUIconnected = false; 
	}

	void Renderer::updateGUIData(const GUIData* guidata) {
		if (!_isGUIconnected) {
			std::cerr << "GUI is not connected!" << std::endl;
			return;
		}

		_translations.clear();
		_rotations.clear();

		for (unsigned int i = 0; i < _shapes.size(); i++) {
			_translations.push_back(glm::translate(glm::mat4(1.0f), guidata->centers[i]));
			_rotations.push_back(ShapeManipulator::getRotationMatrix(guidata->centers[i], guidata->angles[i]));
		}

		_mainShader->setUniformVectorMat4("u_GUISeperateTranslate", _translations);
		_mainShader->setUniformVectorMat4("u_GUISeperateRotation", _rotations);

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
}