/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Mesh.h"

namespace mar {
    namespace graphics {


		void Mesh::createMesh() {
			_shapes = std::vector<std::shared_ptr<Shape>>();
			_vertices = std::vector<float>();
			_indices = std::vector<unsigned int>();
			_samplers = std::vector<int>();
			_translations = std::vector<glm::mat4>();
			_rotations = std::vector<glm::mat4>();

			_light = Light();

			_maxValue = 0;
			_nextShapeID = 0.0f;
		}

		void Mesh::pushShape(std::shared_ptr<Shape>& new_shape) {
			if (_shapes.size() == constants::maxObjectsInScene - 1) {
				std::cout << "Cannot push more objects!" << std::endl;
				return;
			}

			ShapeManipulator::extendShapeID(new_shape, _nextShapeID);
			_nextShapeID++;

			ShapeManipulator::changeIndicesFormat(new_shape, _maxValue);

			_maxValue += new_shape->getSizeofVertices() / new_shape->getStride();

			if (new_shape->getTextureID() != 0.0f)
				_samplers.push_back((int)new_shape->getTextureID());

			_names.push_back(new_shape->getName());

			_shapes.push_back(new_shape);
		}

		void Mesh::popShape(const unsigned int& index) {
			if (_shapes[index]->getTextureID() != 0.0f) {
				_samplers.erase(_samplers.begin() + index);
			}

			_names.erase(_names.begin() + index);

			_shapes[index].reset();
			_shapes.erase(_shapes.begin() + index);
		}

		void Mesh::pushMatrices(const glm::vec3& center, const glm::vec3& angle) {
			_translations.push_back(glm::translate(glm::mat4(1.0f), center));
			_rotations.push_back(ShapeManipulator::getRotationMatrix(center, angle));
		}

		void Mesh::popMatrices(const unsigned int& index) {
			_translations.erase(_translations.begin() + index);
			_rotations.erase(_rotations.begin() + index);
		}

		void Mesh::clearBuffers() {
			_vertices.clear();
			_indices.clear();
		}

		void Mesh::clearMatrices() {
			_translations.clear();
			_rotations.clear();
		}

		void Mesh::update() {
			for (unsigned int i = 0; i < _shapes.size(); i++) {

				unsigned int currentVerticesSize = _vertices.size() + _shapes[i]->getSizeofVertices();
				unsigned int currentIndicesSize = _indices.size() + _shapes[i]->getSizeofIndices();

				if (currentVerticesSize <= constants::maxVertexCount && currentIndicesSize <= constants::maxIndexCount) {
					_vertices.insert(_vertices.end(), _shapes[i]->getVerticesBegin(), _shapes[i]->getVerticesEnd());
					_indices.insert(_indices.end(), _shapes[i]->getIndicesBegin(), _shapes[i]->getIndicesEnd());
				}
			}
		}


} }