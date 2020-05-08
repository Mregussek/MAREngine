/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Renderer.h"

namespace mar {

	void Renderer::initializeRenderer(const std::shared_ptr<RendererFactory>& factory) {
		if (!_initialized) {
			_vbo = factory->createVertexBuffer();
			_lay = factory->createVertexBufferLayout();
			_vao = factory->createVertexArray();
			_ebo = factory->createElementBuffer();
			_texture = factory->createTexture();
			_shader = factory->createShader();

			_pushedOnce = false;
			_maxValue = 0;

			glfwSwapInterval(1);
			glEnable(GL_DEPTH_TEST);
		}
		else {
			std::cerr << "Renderer is already initialized!\n";
		}
	}

	void Renderer::closeRenderer() {
		_shader->shutdown();
		_vbo->close();
		_ebo->close();
	}
	
	void Renderer::initialize(const std::string& filePath) {
		_shader->initialize(filePath);
		_vao->initializeArrayBuffer();
		_vbo->initializeVertex(constants::maxCubeCount);
		_ebo->initializeElement(_indices, constants::maxIndexCount);

		_vao->addBuffer(_lay);

		for (unsigned int i = 0; i < _shapes.size(); i++)
			_texture->bind(_shapes[i]->getID(), _texture->getID(i));

		_shader->bind();
		_shader->setUniformSampler2D("u_Texture", getSamplers());
	}

	void Renderer::pushObject(Shapes* shape, glm::vec3& position, std::string& texturePath) {
		if (shape->getSizeofIndices() + _indices.size() > constants::maxIndexCount) {
			std::cout << "Cannot insert more indices!!!\n";
			return;
		}
		else if (shape->getSizeofVertices() + _vertices.size() > constants::maxVertexCount) {
			std::cout << "Cannot insert more vertices!!!\n";
			return;
		}

		Mesh::extendID(shape, (float)_shapes.size()); // more objects, more texture indexes

		Mesh::changeCenterOfObject(shape, position); // user sends new center position, we need to change vertices

		_vertices.insert(_vertices.end(), shape->getVerticesBegin(), shape->getVetricesEnd()); // insert object vertices to mesh vertices (batch rendering)

		Mesh::changeIndicesFormat(shape, _maxValue); // we cannot use the same indices for the another vertices, that's why we increase them

		_maxValue += shape->getSizeofVertices() / shape->getStride(); // maximum value of indices

		_indices.insert(_indices.end(), shape->getIndicesBegin(), shape->getIndicesEnd()); // insert object indices to mesh indices (batch rendering) 

		_shapes.emplace_back(shape); // place new shape at the end of vector

		_texture->loadTexture(texturePath); // load texture for this object

		_samplers.emplace_back(shape->getID()); // prescribe id to this texture

		if (!_pushedOnce) { // push layout, all objects has the same format, so we need to do it once
			for (size_t i = 0; i < shape->getLayoutSize(); i++)
				_lay->push(shape->getLayout(i), PushBuffer::PUSH_FLOAT);

			_pushedOnce = true;
		}
	}

	void Renderer::popObject() {

	}

	void Renderer::bind() {
		_shader->bind();
		_vao->bind();
		_ebo->bind();
	}

	void Renderer::unbind() {
		_shader->unbind();
		_vao->unbind();
		_vbo->unbind();
		_ebo->unbind();
	}

	void Renderer::updateFrame(const std::vector<glm::vec3>& newCenters, const std::vector<glm::vec3>& newAngles) {
		_vbo->bind(); // set dynamic vertex buffer
		_vertices.clear(); // we are gonna put here new vertices

		for (unsigned int i = 0; i < _shapes.size(); i++) {
			Mesh::changeCenterOfObject(_shapes[i], newCenters[i]);
			Mesh::rotateObject(_shapes[i], newAngles[i]);
			_vertices.insert(_vertices.end(), _shapes[i]->getVerticesBegin(), _shapes[i]->getVetricesEnd());
		}

		_vbo->updateDynamically(_vertices); // now we need only to render
	}

	void Renderer::setGUImatrices(const float* colors, const glm::mat4& translationMatrix, const glm::mat4& rotationMatrix) {
		_shader->setUniform4fv("u_GUIcolor", colors);
		_shader->setUniformMat4f("u_GUItranslation", translationMatrix);
		_shader->setUniformMat4f("u_GUIrotation", rotationMatrix);
	}

	void Renderer::setCameraMatrices(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model) {
		_shader->setUniformMat4f("u_Projection", projection);
		_shader->setUniformMat4f("u_View", view);
		_shader->setUniformMat4f("u_Model", model);
	}

	void Renderer::draw() const {
		if (_indices.size() == 0)
			glDrawArrays(GL_TRIANGLES, 0, _vbo->getSize());
		else
			glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::clear() {
		glClearColor(0.75f, 0.75f, 0.75f, 1.0f); // light gray
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

}