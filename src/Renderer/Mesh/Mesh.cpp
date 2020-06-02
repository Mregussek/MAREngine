/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Mesh.h"

namespace mar {
    namespace graphics {

		Mesh::~Mesh() {
			m_texture->shutdown();
		}

		void Mesh::createMesh(const std::shared_ptr<RendererFactory>& factory) {
			m_texture = factory->createTexture();

			m_shapes = std::vector<std::shared_ptr<Shape>>();
			m_vertices = std::vector<float>();
			m_indices = std::vector<unsigned int>();
			m_samplers = std::vector<int>();
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
			if (m_shapes.size() == constants::maxObjectsInScene - 1) {
				std::cout << "Cannot push more objects!" << std::endl;
				return;
			}
			
			pushTexture(new_shape, texture);
			pushShape(new_shape);
			pushMatrices(center, angle);
		}

		void Mesh::flushShape(const unsigned int& index) {
			popShape(index);
		}

		void Mesh::pushTexture(std::shared_ptr<Shape>& new_shape, const std::string& texture) {
			if (texture != "empty") {
				ShapeManipulator::extendTextureID(new_shape, m_availableTextureID);

				m_samplers.push_back((int)m_availableTextureID);

				m_availableTextureID++;

				m_texture->loadTexture(texture);
			}
			else {
				new_shape->setTextureID(0.f);
			}
		}

		void Mesh::pushShape(std::shared_ptr<Shape>& new_shape) {
			ShapeManipulator::extendShapeID(new_shape, m_availableShapeID);
			m_availableShapeID++;

			ShapeManipulator::changeIndicesFormat(new_shape, m_indicesMaxValue);

			m_indicesMaxValue += new_shape->getSizeofVertices() / new_shape->getStride();

			m_names.push_back(new_shape->getName());

			m_shapes.push_back(new_shape);
		}

		void Mesh::pushMatrices(const glm::vec3& center, const glm::vec3& angle) {
			m_translationMats.push_back(glm::translate(glm::mat4(1.0f), center));

			glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(angle.x), glm::vec3(1.0f, 0.0f, 0.0f))
				* glm::rotate(glm::mat4(1.0f), glm::radians(angle.y), glm::vec3(0.0f, 1.0f, 0.0f))
				* glm::rotate(glm::mat4(1.0f), glm::radians(angle.z), glm::vec3(0.0f, 0.0f, 1.0f));

			m_rotationMats.push_back(transform);
		}

		void Mesh::popShape(const unsigned int& index) {
			if (m_shapes[index]->getTextureID() != 0.0f) {
				unsigned int sizeDiff = m_shapes.size() - m_samplers.size();
				std::vector<int> new_samplers;

				for (unsigned int i = 0; i < m_samplers.size() ; i++)
					if (i != index - sizeDiff)
						new_samplers.push_back(m_samplers[i]);
				
				m_samplers = new_samplers;

				m_texture->removeID(index - sizeDiff);
			}

			std::vector<std::string> new_names;
			std::vector<std::shared_ptr<Shape>> new_shapes;
			std::vector<glm::mat4> new_translations;
			std::vector<glm::mat4> new_rotations;

			for (unsigned int i = 0; i < m_names.size(); i++) {
				if (i != index) {
					new_names.push_back(m_names[i]);
					new_shapes.push_back(m_shapes[i]);
					new_translations.push_back(m_translationMats[i]);
					new_rotations.push_back(m_rotationMats[i]);
				}
				
				if(i == index)
					m_shapes[i].reset();
			}

			m_names = new_names;
			m_shapes = new_shapes;
			m_translationMats = new_translations;
			m_rotationMats = new_rotations;
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
			for (unsigned int i = 0; i < m_shapes.size(); i++) {

				unsigned int currentVerticesSize = m_vertices.size() + m_shapes[i]->getSizeofVertices();
				unsigned int currentIndicesSize = m_indices.size() + m_shapes[i]->getSizeofIndices();

				if (currentVerticesSize <= constants::maxVertexCount && currentIndicesSize <= constants::maxIndexCount) {
					auto beginVert = m_shapes[i]->getVerticesBegin();
					auto endVert = m_shapes[i]->getVerticesEnd();
					auto beginIndices = m_shapes[i]->getIndicesBegin();
					auto endIndices = m_shapes[i]->getIndicesEnd();
					
					m_vertices.insert(m_vertices.end(), beginVert, endVert);
					m_indices.insert(m_indices.end(), beginIndices, endIndices);
				}
			}

			for (unsigned int i = 0; i < m_samplers.size(); i++)
				m_texture->bind(m_samplers[i], m_texture->getID(i));
		}


} }