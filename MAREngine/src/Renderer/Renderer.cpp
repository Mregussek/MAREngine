/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Renderer.h"

namespace mar {

	Renderer::~Renderer() {
		_vbo->close();
		_ebo->close();
	}

	void Renderer::initializeRenderer(const std::shared_ptr<RendererFactory>& factory) {
		if (!_initialized) {
			_vbo = factory->createVertexBuffer();
			_lay = factory->createVertexBufferLayout();
			_vao = factory->createVertexArray();
			_ebo = factory->createElementBuffer();
			_texture = factory->createTexture();

			_pushedOnce = false;
			_maxValue = 0;

			glfwSwapInterval(1);
			glEnable(GL_DEPTH_TEST);
		}
		else {
			std::cerr << "Renderer is already initialized!\n";
		}
	}

	void Renderer::initializeBuffers() {
		_vbo->initializeVertex(constants::maxCubeCount);
		_ebo->initializeElement(_indices, constants::maxIndexCount);

		_vao->addBufferPointers(_lay);

		for (unsigned int i = 0; i < _shapes.size(); i++)
			_texture->bind(_shapes[i].getID(), _texture->getID(i));
	}

	void Renderer::pushObject(Cube* cube, glm::vec3& position, std::string& texturePath) {
		if (cube->indicesVector.size() + _indices.size() > constants::maxIndexCount) {
			std::cout << "Cannot insert more indices!!!\n";
			return;
		}
		else if (cube->verticesVector.size() + _vertices.size() > constants::maxVertexCount) {
			std::cout << "Cannot insert more vertices!!!\n";
			return;
		}

		Mesh::extendID(cube, (float)_shapes.size()); // more objects, more texture indexes

		Mesh::changeCenterOfObject(cube, position); // user sends new center position, we need to change vertices

		_vertices.insert(_vertices.end(), cube->verticesVector.begin(), cube->verticesVector.end()); // insert object vertices to mesh vertices (batch rendering)

		Mesh::changeIndicesFormat(cube, _maxValue); // we cannot use the same indices for the another vertices, that's why we increase them

		_maxValue += cube->getSizeofVertices() / cube->getStride(); // maximum value of indices

		_indices.insert(_indices.end(), cube->indicesVector.begin(), cube->indicesVector.end()); // insert object indices to mesh indices (batch rendering) 

		_shapes.emplace_back(*cube); // place new shape at the end of vector

		_texture->loadTexture(texturePath); // load texture for this object

		_samplers.emplace_back(cube->getID()); // prescribe id to this texture

		if (!_pushedOnce) { // push layout, all objects has the same format, so we need to do it once
			for (auto const& l : cube->layout)
				_lay->push<float>(l);

			_pushedOnce = true;
		}
	}

	void Renderer::popObject() {

	}

	void Renderer::bind() {
		_vao->bind();
		_ebo->bind();
	}

	void Renderer::unbind() {
		_vao->unbind();
		_vbo->unbind();
		_ebo->unbind();
	}

	void Renderer::updateFrame(const std::vector<glm::vec3>& newCenters, const std::vector<glm::vec3>& newAngles) {
		_vbo->bind(); // set dynamic vertex buffer
		_vertices.clear(); // we are gonna put here new vertices

		for (unsigned int i = 0; i < _shapes.size(); i++) {
			Mesh::changeCenterOfObject(&_shapes[i], newCenters[i]);
			Mesh::rotateObject(&_shapes[i], newAngles[i]);
			_vertices.insert(_vertices.end(), _shapes[i].verticesVector.begin(), _shapes[i].verticesVector.end());
		}

		_vbo->updateDynamically(_vertices); // now we need only to render
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