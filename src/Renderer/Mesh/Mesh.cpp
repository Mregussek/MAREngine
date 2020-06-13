/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Mesh.h"
#include "../../Debug/Log.h"


namespace mar {
    namespace graphics {

		float Mesh::s_availableTextureID = 1.0f;

		Mesh::~Mesh() {
			m_texture->shutdown();

			clearMatrices();
			clearBuffers();

			m_samplers.clear();

			for (auto& s : m_shapes)
				s.reset();

			m_shapes.clear();
		}

		void Mesh::createMesh(const Ref<RendererFactory>& factory) {
			m_texture = factory->createTexture();

			m_shapes = std::vector<Ref<Shape>>();
			m_vertices = std::vector<float>();
			m_indices = std::vector<unsigned int>();
			m_samplers = std::vector<int>();
			m_translationMats = std::vector<glm::mat4>();
			m_rotationMats = std::vector<glm::mat4>();

			m_light = Light();

			m_indicesMaxValue = 0;
			m_availableShapeID = 0.0f;

			MAR_CORE_INFO("Mesh has been created!");
		}

		void Mesh::loadScene(Scene* scene, MeshTextures type) {
			unsigned int shapesInSceneCount = scene->getShapesNumber();

			switch(type) {
			case MeshTextures::TEXTURES:
				for (unsigned int i = 0; i < shapesInSceneCount; i++)
					submitShape(scene->getShape(i), scene->getCenter(i), scene->getAngle(i), scene->getTexture(i));

				MAR_CORE_INFO("Scene has been loaded by textures!");

				break;

			case MeshTextures::CUBEMAPS:
				for (unsigned int i = 0; i < shapesInSceneCount; i++)
					submitShape(scene->getShape(i), scene->getCenter(i), scene->getAngle(i), scene->getFace(i));

				MAR_CORE_INFO("Scene has been loaded by textures!");

				break;
			}
		}

		void Mesh::submitShape(Ref<Shape>& new_shape, const glm::vec3& center, const glm::vec3& angle, const std::string& texture) {
			if (m_shapes.size() == constants::maxObjectsInScene - 1) {
				std::cout << "Cannot push more objects!" << std::endl;
				return;
			}
			
			new_shape->setCenter(center);
			new_shape->setAngle(angle);

			pushTexture(new_shape, texture);
			pushShape(new_shape);
			pushMatrices(center, angle);

			MAR_CORE_INFO("Added new object to scene!");
		}

		void Mesh::submitShape(Ref<Shape>& new_shape, const glm::vec3& center, const glm::vec3& angle, const std::vector<std::string>& faces) {
			if (m_shapes.size() == constants::maxObjectsInScene - 1) {
				std::cout << "Cannot push more objects!" << std::endl;
				return;
			}

			new_shape->setCenter(center);
			new_shape->setAngle(angle);

			pushCubeMap(new_shape, faces);
			pushShape(new_shape);
			pushMatrices(center, angle);

			MAR_CORE_INFO("Added new object to scene!");
		}

		void Mesh::flushShape(const unsigned int& index) {
			if (m_shapes.size() <= 1) {
				MAR_CORE_ERROR("Cannot delete the last shape!");
				return;
			}

			popShape(index);

			MAR_CORE_INFO("Deleted object from scene!");
		}

		void Mesh::pushTexture(Ref<Shape>& new_shape, const std::string& texture) {
			if (texture != "empty") {
				ShapeManipulator::extendTextureID(new_shape, s_availableTextureID);

				m_samplers.push_back((int)s_availableTextureID);

				s_availableTextureID++;

				m_texture->loadTexture(texture);
			}
			else {
				new_shape->setTextureID(0.f);

				m_samplers.push_back(0);

				m_texture->addID(0);
			}
		}

		void Mesh::pushCubeMap(Ref<Shape>& new_shape, const std::vector<std::string>& faces) {
			ShapeManipulator::extendTextureID(new_shape, s_availableTextureID);

			m_samplers.push_back((int)s_availableTextureID);

			s_availableTextureID++;

			m_texture->loadCubemap(faces);
		}

		void Mesh::pushShape(Ref<Shape>& new_shape) {
			ShapeManipulator::extendShapeID(new_shape, m_availableShapeID);
			m_availableShapeID++;

			ShapeManipulator::changeIndicesFormat(new_shape, m_indicesMaxValue);

			m_indicesMaxValue += new_shape->getSizeofVertices() / new_shape->getStride();

			m_shapes.push_back(new_shape);
		}

		void Mesh::pushMatrices(const glm::vec3& center, const glm::vec3& angle) {
			m_translationMats.push_back(glm::translate(glm::mat4(1.0f), center));

			glm::mat4 transform = 
				  glm::rotate(glm::mat4(1.0f), glm::radians(angle.x), glm::vec3(1.0f, 0.0f, 0.0f))
				* glm::rotate(glm::mat4(1.0f), glm::radians(angle.y), glm::vec3(0.0f, 1.0f, 0.0f))
				* glm::rotate(glm::mat4(1.0f), glm::radians(angle.z), glm::vec3(0.0f, 0.0f, 1.0f));

			m_rotationMats.push_back(transform);
		}

		void Mesh::popShape(const unsigned int& index) {
			unsigned int min_value = m_shapes[index]->getMinValueOfIndices();
			unsigned int max_value = m_shapes[index]->getMaxValueOfIndices();
			int diff = (-1) * (max_value - min_value + 1);

			for (unsigned int i = index; i < m_shapes.size() - 1; i++) {
				ShapeManipulator::changeIndicesFormat(m_shapes[i + 1], diff);
				ShapeManipulator::extendShapeID(m_shapes[i + 1], m_shapes[i + 1]->getID() - 1.f);

				MeshCreator::moveShape(m_shapes[i], m_shapes[i + 1]);
			}

			m_availableShapeID--;
			
			m_shapes[m_shapes.size() - 1].reset();
			m_shapes.pop_back();
			m_samplers.pop_back();
			m_texture->removeID(index);
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