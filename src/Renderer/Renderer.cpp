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
			_shader = factory->createShader();

			_shapes = std::make_shared<std::vector<std::shared_ptr<Shapes>>>();
			_stats = RendererStatistics();
			_pushedOnce = false;
			_runtime = false;
			_maxValue = 0;
			_startupSceneSize = 0;
			_helperIndex = 0;
		}
		else {
			std::cerr << "Renderer is already initialized!\n";
		}
	}

	void Renderer::closeRenderer() {
		_shader->shutdown();
		_texture->shutdown();
		_vao->closeArrayBuffer();
		_vbo->close();
		_ebo->close();
	}
	
	void Renderer::initialize() {
		_shader->initialize();
		_vao->initializeArrayBuffer();
		_vbo->initializeVertex(constants::maxVertexCount);
		_ebo->initializeElement(constants::maxIndexCount);

		_vao->addBuffer(_lay);

		for (unsigned int i = 0; i < _shapes->size(); i++) {
			_texture->bind(_shapes->at(i)->getID(), _texture->getID(i));
		}
			
		_shader->bind();

		_startupSceneSize = _shapes->size();
		_runtime = true;
	}

	///! TODO: When you:
	///! 1. Add new object to scene
	///! 2. Delete this object from scene
	///! 3. Just add new object to scene
	///! Program crashes because next texture id is being increased, and the number is not correct
	///! That's why we have seg fault. We have to investigate, why after loading scene we have 
	///! increased value
	///! Also look at TODO at popObject() method. There is associated problem!
	void Renderer::pushObject(std::shared_ptr<Shapes>& shape, glm::vec3& position, std::string texturePath) {
		if (_shapes->size() == constants::maxObjectsInScene) {
			std::cout << "Cannot push more objects!\n";
			return;
		}

		Mesh::extendID(shape, (float)_shapes->size());

		Mesh::changeCenterOfObject(shape, position);

		Mesh::changeIndicesFormat(shape, _maxValue);

		_maxValue += shape->getSizeofVertices() / shape->getStride();

		_shapes->emplace_back(shape);

		_texture->loadTexture(texturePath);

		_samplers.push_back(shape->getID());

		if (!_pushedOnce) {
			for (size_t i = 0; i < shape->getLayoutSize(); i++)
				_lay->push(shape->getLayout(i), PushBuffer::PUSH_FLOAT);

			_pushedOnce = true;
		}
	}

	///! TODO: When you:
	///! 1. Push a few objects to scene (for instance 5)
	///! 2. Delete object with index 0
	///! 3. and then again index 0 (This index was one, but because of vector reshape now is 0)
	///! program crashes, because of memory leak. We cannot use the same memory address for new object
	///! Maybe we should delete existing vector and copy its whole stuff to new one?
	///! Or use shapes as shared_ptr's, and with reset() method push new element and delete existing ones?
	void Renderer::popObject(const unsigned int& index) {
		_shapes->at(index).reset();
		_shapes->erase(_shapes->begin() + index);
		_helperIndex--;

		_samplers.erase(_samplers.begin() + index);
		_texture->removeID(index);

		_vertices.clear();
		_indices.clear();
	}

	void Renderer::bind() {
		_shader->bind();
		_vao->bind();
		_vbo->bind();
		_ebo->bind();
	}

	void Renderer::unbind() {
		_shader->unbind();
		_vao->unbind();
		_vbo->unbind();
		_ebo->unbind();
	}

	///! TODO: Definitely need to work on this loop, when there is no memory, which we need there is still no draw
	///! We have one draw call, because we have this memory available, but when we need second draw call program crashes!
	void Renderer::updateFrame() {
		clear();
		bind();

		// Reset statistics before drawing
		_stats._countOfDrawCalls = 0;
		_stats._countOfIndices = 0;
		_stats._countOfShapes = 0;
		_stats._countOfVertices = 0;

		// Clear buffer
		_vertices.clear();
		_indices.clear();

		// Main Loop - Batch Rendering
		for (unsigned int i = 0; i < _shapes->size(); i++) {
			if (_vertices.size() + _shapes->at(i)->getSizeofVertices() <= constants::maxVertexCount 
							&& _indices.size() + _shapes->at(i)->getSizeofIndices() <= constants::maxIndexCount) {

				_vertices.insert(_vertices.end(), _shapes->at(i)->getVerticesBegin(), _shapes->at(i)->getVerticesEnd());
				_indices.insert(_indices.end(), _shapes->at(i)->getIndicesBegin(), _shapes->at(i)->getIndicesEnd());

				_stats._countOfVertices += _shapes->at(i)->getSizeofVertices();
				_stats._countOfIndices += _shapes->at(i)->getSizeofIndices();
				_stats._countOfShapes++;
			} 
			else {
				_vbo->updateDynamically(_vertices);
				_ebo->updateDynamically(_indices);

				draw();

				_stats._countOfDrawCalls++;

				_vertices.clear();
				_indices.clear();
			}
		}

		_vbo->updateDynamically(_vertices);
		_ebo->updateDynamically(_indices);
		draw();
		_stats._countOfDrawCalls++;

		for (unsigned int i = 0; i < _shapes->size(); i++) {
			_texture->bind(_shapes->at(i)->getID(), _texture->getID(i));
		}
			
	}

	void Renderer::draw() {
		// --- GUI UNIFORMS --- //
		_shader->setUniform4fv("u_GUIcolor", _gui_colors);
		_shader->setUniformMat4f("u_GUItranslation", _gui_translate);
		_shader->setUniformMat4f("u_GUIrotation", _gui_rotation);

		// --- CAMERA UNIFORMS --- //
		_shader->setUniformMat4f("u_Projection", _camera_projection);
		_shader->setUniformMat4f("u_View", _camera_view);
		_shader->setUniformMat4f("u_Model", _camera_model);

		// --- RENDERING UNIFORMS --- //
		_shader->setUniformVectorMat4("u_RenderTranslate", _translations);
		_shader->setUniformVectorMat4("u_RenderRotation", _rotations);

		// --- LIGHTS UNIFORMS --- //		
		_shader->setUniformVector3("u_LightPos", _lightPosition);
		_shader->setUniformVector3("u_CameraPos", _camera_position);

		// --- TEXTURE UNIFORMS --- //
		_shader->setUniformSampler2D("u_Texture", getSamplers());

		// --- MAIN DRAW CALL --- // 
		glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::clear() {
		glClearColor(0.85f, 0.85f, 0.85f, 1.0f); // light gray
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::setGUIvectors(const std::vector<glm::vec3>& newCenters, const std::vector<glm::vec3>& newAngles) {
		_translations.clear();
		_rotations.clear();

		for (unsigned int i = 0; i < _shapes->size(); i++) {
			_translations.push_back(glm::translate(glm::mat4(1.0f), newCenters[i]));
			_rotations.push_back(Mesh::getRotationMatrix(newCenters[i], newAngles[i]));
		}
	}

	void Renderer::setGUImatrices(const float* colors, const glm::mat4& translationMatrix, const glm::mat4& rotationMatrix) {
		_gui_colors = colors;
		_gui_translate = translationMatrix;
		_gui_rotation = rotationMatrix;
	}

	void Renderer::setCameraMatrices(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model) {
		_camera_projection = projection;
		_camera_view = view;
		_camera_model = model;
	}

	void Renderer::setCameraVectors(const glm::vec3& position) {
		_camera_position = position;
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

	const std::string& Renderer::getObjectName(unsigned int index) { 
		return _shapes->at(index)->getName();
	}

	const std::vector<int>& Renderer::getSamplers() const { 
		return _samplers; 
	}

	const RendererStatistics& Renderer::getStatistics() const {
		return _stats;
	}

	const unsigned int& Renderer::getSceneStartupSize() const {
		return _startupSceneSize;
	}
}