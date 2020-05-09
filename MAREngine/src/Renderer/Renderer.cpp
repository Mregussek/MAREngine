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

			_pushedOnce = false;
			_maxValue = 0;
			_countOfDrawCalls = 0;
			_countOfIndices = 0;
			_countOfShapes = 0;
			_countOfVertices = 0;
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
	
	void Renderer::initialize(const std::string& filePath) {
		_shader->initialize(filePath);
		_vao->initializeArrayBuffer();
		_vbo->initializeVertex(constants::maxVertexCount);
		_ebo->initializeElement(constants::maxIndexCount);

		_vao->addBuffer(_lay);

		for (unsigned int i = 0; i < _shapes.size(); i++)
			_texture->bind(_shapes[i]->getID(), _texture->getID(i));

		_shader->bind();
		_shader->setUniformSampler2D("u_Texture", getSamplers());
	}

	void Renderer::pushObject(Shapes* shape, glm::vec3& position, std::string texturePath) {
		Mesh::extendID(shape, (float)_shapes.size()); // more objects, more texture indexes

		Mesh::changeCenterOfObject(shape, position); // user sends new center position, we need to change vertices

		Mesh::changeIndicesFormat(shape, _maxValue); // we cannot use the same indices for the another vertices, that's why we increase them

		_maxValue += shape->getSizeofVertices() / shape->getStride(); // maximum value of indices

		_shapes.emplace_back(shape); // place new shape at the end of vector

		_texture->loadTexture(texturePath); // load texture for this object

		_samplers.emplace_back(shape->getID()); // prescribe id to this texture

		if (!_pushedOnce) { // push layout, all objects has the same format, so we need to do it once
			for (size_t i = 0; i < shape->getLayoutSize(); i++)
				_lay->push(shape->getLayout(i), PushBuffer::PUSH_FLOAT);

			_pushedOnce = true;
		}
	}

	void Renderer::popObject(const unsigned int& index) {
		_vertices.clear();
		_indices.clear();

		_shapes.erase(_shapes.begin() + index);
		_samplers.erase(_samplers.begin() + index);
		_texture->removeID(index);
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

	void Renderer::updateFrame() {
		// Prepare screen for new frame
		clear();
		bind();

		// Reset statistics before drawing
		_countOfDrawCalls = 0;
		_countOfIndices = 0;
		_countOfShapes = 0;
		_countOfVertices = 0;

		// Clear buffer
		_vertices.clear();
		_indices.clear();

		///! TODO: Definitely need to work on this loop, when there is no memory, which we need there is still no draw

		// Main Loop - Batch Rendering
		for (unsigned int i = 0; i < _shapes.size(); i++) {
			// If we can, we put new vertices and indices to vector, which will be drawn
			if (_vertices.size() + _shapes[i]->getSizeofVertices() <= constants::maxVertexCount 
							&& _indices.size() + _shapes[i]->getSizeofIndices() <= constants::maxIndexCount) { 

				_vertices.insert(_vertices.end(), _shapes[i]->getVerticesBegin(), _shapes[i]->getVerticesEnd());
				_indices.insert(_indices.end(), _shapes[i]->getIndicesBegin(), _shapes[i]->getIndicesEnd());

				_countOfVertices += _shapes[i]->getSizeofVertices();
				_countOfIndices += _shapes[i]->getSizeofIndices();
				_countOfShapes++;
			} 
			else {
				// If we cannot, we must draw everything and then collect data from the rest
				_vbo->updateDynamically(_vertices);
				_ebo->updateDynamically(_indices);

				draw();

				_countOfDrawCalls++;

				_vertices.clear();
				_indices.clear();
			}
		}

		// This is the "one" draw, if can put all data to one vector,
		// if we cannot it is the last draw call
		_vbo->updateDynamically(_vertices);
		_ebo->updateDynamically(_indices);
		draw();
		_countOfDrawCalls++;
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

		glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::clear() {
		glClearColor(0.85f, 0.85f, 0.85f, 1.0f); // light gray
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::setGUIvectors(const std::vector<glm::vec3>& newCenters, const std::vector<glm::vec3>& newAngles) {
		_translations.clear();
		_rotations.clear();

		for (unsigned int i = 0; i < _shapes.size(); i++) {
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

	void Renderer::guiPushPyramid(glm::vec3& position) {
		pushObject(&Pyramid(), position);
	}

	void Renderer::guiPushCube(glm::vec3& position) {
		pushObject(&Cube(), position);
	}

	void Renderer::guiPushSurface(glm::vec3& position) {
		pushObject(&Surface(), position);
	}

	const std::vector<int>& Renderer::getSamplers() const { 
		return _samplers; 
	}

	const std::vector<unsigned int> Renderer::getStatistics() const {
		return {_countOfDrawCalls, _countOfShapes, _countOfVertices, _countOfIndices};
	}
}