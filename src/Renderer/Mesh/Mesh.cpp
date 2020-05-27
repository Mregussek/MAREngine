/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Mesh.h"

namespace mar {

	void Mesh::initialize() {
		_shapes = std::vector<std::shared_ptr<Shape>>();
		_pushedLayout = false;
		_maxValue = 0;
		_nextShapeID = 0.0f;
		_nextTextureID = 1.0f; // from 1.0f, cause 0.0f is reserved for default color
	}

    void Mesh::createMesh(Scene* scene) {
		for (unsigned int i = 0; i < scene->getShapesNumber(); i++) {
			pushObject(scene->getShape(i), scene->getCenter(i), scene->getTexture(i));
			//_translations.push_back(glm::translate(glm::mat4(1.0f), scene->getCenter(i)));
			//_rotations.push_back(ShapeManipulator::getRotationMatrix(scene->getCenter(i), scene->getAngle(i)));
		}
    }

	void Mesh::deleteMesh() {

	}

	void Mesh::pushObject(std::shared_ptr<Shape>& shape, glm::vec3& position, std::string texturePath) {
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

	void Mesh::popObject(const unsigned int& index) {
		if (_shapes[index]->getTextureID() != 0.0f) {
			_samplers.erase(_samplers.begin() + index);
			_texture->removeID(index);
		}

		_shapes[index].reset();
		_shapes.erase(_shapes.begin() + index);

		_vertices.clear();
		_indices.clear();
	}

	void Mesh::bind() {
		for (unsigned int i = 0; i < _samplers.size(); i++)
			_texture->bind((float)_samplers[i], _texture->getID(i));
	}

	void Mesh::unbind() {
		for (unsigned int i = 0; i < _samplers.size(); i++)
			_texture->bind((float)_samplers[i], 0.0f);
	}

	void Mesh::insertVertices(const std::shared_ptr<Shape>& shape) {
		_vertices.insert(_vertices.end(), shape->getVerticesBegin(), shape->getVerticesEnd());
	}

	void Mesh::insertIndices(const std::shared_ptr<Shape>& shape) {
		_indices.insert(_indices.end(), shape->getIndicesBegin(), shape->getIndicesEnd());
	}
}