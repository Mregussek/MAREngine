/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Mesh.h"

namespace mar {
    namespace graphics {

		Mesh::~Mesh() {
			_texture->shutdown();
		}

		void Mesh::createMesh(const std::shared_ptr<RendererFactory>& factory) {
			_texture = factory->createTexture();

			_shapes = std::vector<std::shared_ptr<Shape>>();
			m_vertices = std::vector<float>();
			m_indices = std::vector<unsigned int>();
			_samplers = std::vector<int>();
			m_translationMats = std::vector<glm::mat4>();
			m_rotationMats = std::vector<glm::mat4>();

			_light = Light();

			m_indicesMaxValue = 0;
			m_availableShapeID = 0.0f;
			m_availableTextureID = 1.0f;
		}

		void Mesh::loadScene(Scene* scene) {
			unsigned int shapesInSceneCount = scene->getShapesNumber();

			for (unsigned int i = 0; i < shapesInSceneCount; i++)
				submitShape(scene->getShape(i), scene->getCenter(i), scene->getAngle(i), scene->getTexture(i));
		}

		void Mesh::submitShape(std::shared_ptr<Shape>& new_shape, const glm::vec3& center, const glm::vec3& angle, const std::string& texture) {
			pushTexture(new_shape, texture);
			pushShape(new_shape);
			pushMatrices(center, angle);
		}

		void Mesh::flushShape(const unsigned int& index) {
			popShape(index);
			popMatrices(index);
		}

		void Mesh::pushShape(std::shared_ptr<Shape>& new_shape) {
			if (_shapes.size() == constants::maxObjectsInScene - 1) {
				std::cout << "Cannot push more objects!" << std::endl;
				return;
			}

			ShapeManipulator::extendShapeID(new_shape, m_availableShapeID);
			m_availableShapeID++;

			ShapeManipulator::changeIndicesFormat(new_shape, m_indicesMaxValue);

			m_indicesMaxValue += new_shape->getSizeofVertices() / new_shape->getStride();

			_names.push_back(new_shape->getName());

			_shapes.push_back(new_shape);
		}

		void Mesh::popShape(const unsigned int& index) {
			if (_shapes[index]->getTextureID() != 0.0f) {
				popTexture(index);
			}

			_names.erase(_names.begin() + index);

			_shapes[index].reset();
			_shapes.erase(_shapes.begin() + index);
		}

		void Mesh::pushMatrices(const glm::vec3& center, const glm::vec3& angle) {
			m_translationMats.push_back(glm::translate(glm::mat4(1.0f), center));
			m_rotationMats.push_back(ShapeManipulator::getRotationMatrix(center, angle));
		}

		void Mesh::popMatrices(const unsigned int& index) {
			m_translationMats.erase(m_translationMats.begin() + index);
			m_rotationMats.erase(m_rotationMats.begin() + index);
		}

		void Mesh::pushTexture(std::shared_ptr<Shape>& new_shape, const std::string& texture) {
			if (texture != "empty") {
				ShapeManipulator::extendTextureID(new_shape, m_availableTextureID);
				m_availableTextureID++;

				_texture->loadTexture(texture);

				_samplers.push_back((int)m_availableTextureID);
			}
		}

		void Mesh::popTexture(const unsigned int& index) {
			_samplers.erase(_samplers.begin() + index);
		}

		void Mesh::clearBuffers() {
			m_vertices.clear();
			m_indices.clear();
		}

		void Mesh::clearMatrices() {
			m_translationMats.clear();
			m_rotationMats.clear();
		}

		void Mesh::update() {
			for (unsigned int i = 0; i < _shapes.size(); i++) {

				unsigned int currentVerticesSize = m_vertices.size() + _shapes[i]->getSizeofVertices();
				unsigned int currentIndicesSize = m_indices.size() + _shapes[i]->getSizeofIndices();

				if (currentVerticesSize <= constants::maxVertexCount && currentIndicesSize <= constants::maxIndexCount) {
					m_vertices.insert(m_vertices.end(), _shapes[i]->getVerticesBegin(), _shapes[i]->getVerticesEnd());
					m_indices.insert(m_indices.end(), _shapes[i]->getIndicesBegin(), _shapes[i]->getIndicesEnd());
				}
			}

			for (unsigned int i = 0; i < _samplers.size(); i++)
				_texture->bind(_samplers[i], _texture->getID(i));
		}


} }