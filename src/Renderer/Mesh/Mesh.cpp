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
			m_shapesCount = 0;

			MAR_CORE_INFO("Mesh has been created!");
		}

		void Mesh::loadScene(Scene* scene, MeshType type) {
			unsigned int shapesInSceneCount = scene->getShapesNumber();
			m_type = type;

			switch(m_type) {
			case MeshType::NORMAL:
				for (unsigned int i = 0; i < shapesInSceneCount; i++)
					submitShape(scene->getShape(i), scene->getCenter(i), scene->getAngle(i), scene->getTexture(i));

				MAR_CORE_INFO("Scene has been loaded by textures!");

				break;

			case MeshType::CUBEMAPS:
				for (unsigned int i = 0; i < shapesInSceneCount; i++)
					submitShape(scene->getShape(i), scene->getCenter(i), scene->getAngle(i), scene->getFace(i));

				MAR_CORE_INFO("Scene has been loaded by cubemaps!");

				break;
			case MeshType::OBJECTS:
				for (unsigned int i = 0; i < shapesInSceneCount; i++) {
					auto shape = scene->getShape(i);
					shape->assignDataFromFile(scene->getObjPath(i));

					submitShape(shape, scene->getCenter(i), scene->getAngle(i), scene->getTexture(i));
				}
					
				MAR_CORE_INFO("Scene has been loaded by objects!");

				break;
			}
		}

		void Mesh::tryReuseShape(Ref<Shape>& new_shape, const glm::vec3& center, const glm::vec3& angle, const char* texture) {
			if (m_shapes.size() != m_shapesCount) {
				pushTexture(new_shape, texture);
				reuse(new_shape, center, angle);

				return;
			}

			submitShape(new_shape, center, angle, texture);
		}

		void Mesh::tryReuseShape(Ref<Shape>& new_shape, const glm::vec3& center, const glm::vec3& angle, const std::vector<const char*>& faces) {
			if (m_shapes.size() != m_shapesCount) {
				pushCubeMap(new_shape, faces);
				reuse(new_shape, center, angle);

				return;
			}

			submitShape(new_shape, center, angle, faces);
		}

		void Mesh::reuse(Ref<Shape>& new_shape, const glm::vec3& center, const glm::vec3& angle) {
			new_shape->setCenter(center);
			new_shape->setAngle(angle);
			pushMatrices(center, angle);

			ShapeManipulator::extendShapeID(new_shape, m_availableShapeID);
			m_availableShapeID++;

			ShapeManipulator::changeIndicesFormat(new_shape, m_indicesMaxValue);
			m_indicesMaxValue += new_shape->getSizeofVertices() / new_shape->getStride();

			MeshCreator::moveShape(m_shapes[m_shapesCount], new_shape);

			m_shapesCount++;

			MAR_CORE_INFO("Reusing shape object!");
		}

		void Mesh::submitShape(Ref<Shape>& new_shape, const glm::vec3& center, const glm::vec3& angle, const char* texture) {
			if (m_shapesCount == constants::maxObjectsInScene - 1) {
				MAR_CORE_ERROR("Cannot push more objects!");
				return;
			}
			
			pushTexture(new_shape, texture);
			submit(new_shape, center, angle);
		}

		void Mesh::submitShape(Ref<Shape>& new_shape, const glm::vec3& center, const glm::vec3& angle, const std::vector<const char*>& faces) {
			if (m_shapesCount == constants::maxObjectsInScene - 1) {
				MAR_CORE_ERROR("Cannot push more objects!");
				return;
			}

			pushCubeMap(new_shape, faces);
			submit(new_shape, center, angle);
		}

		void Mesh::submit(Ref<Shape>& new_shape, const glm::vec3& center, const glm::vec3& angle) {
			new_shape->setCenter(center);
			new_shape->setAngle(angle);
			pushShape(new_shape);
			pushMatrices(center, angle);

			m_shapesCount++;

			MAR_CORE_INFO("Added new object to scene!");
		}

		void Mesh::pushTexture(Ref<Shape>& new_shape, const char* texture) {
			if (texture != "empty") {
				ShapeManipulator::extendTextureID(new_shape, s_availableTextureID);

				m_samplers.push_back((int)s_availableTextureID);

				m_colors.push_back(new_shape->getDefaultColor());

				s_availableTextureID++;

				m_texture->loadTexture(texture);
			}
			else {
				new_shape->setTextureID(0.f);

				m_colors.push_back(new_shape->getDefaultColor());

				m_samplers.push_back(0);

				m_texture->addID(0);
			}
		}

		void Mesh::pushCubeMap(Ref<Shape>& new_shape, const std::vector<const char*>& faces) {
			ShapeManipulator::extendTextureID(new_shape, s_availableTextureID);

			m_samplers.push_back((int)s_availableTextureID);

			m_colors.push_back(new_shape->getDefaultColor());

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

		void Mesh::flushShape(const unsigned int& index) {
			if (m_shapesCount == 1) {
				MAR_CORE_ERROR("Cannot delete the last shape!");
				return;
			}

			popShape(index);
			popMatrices(index);

			MAR_CORE_INFO("Deleted object from scene!");
		}

		void Mesh::popShape(const unsigned int& index) {
			unsigned int min_value = m_shapes[index]->getMinValueOfIndices();
			unsigned int max_value = m_shapes[index]->getMaxValueOfIndices();
			int diff = (-1) * (max_value - min_value + 1);

			m_indicesMaxValue -= m_shapes[index]->getSizeofVertices() / m_shapes[index]->getStride();

			for (unsigned int i = index; i < m_shapesCount - 1; i++) {
				float save_tex_id = m_shapes[i + 1]->getTextureID();
				m_samplers[i] = m_samplers[i + 1];
				m_colors[i] = m_colors[i + 1];
				MeshCreator::moveShape(m_shapes[i], m_shapes[i + 1]);

				m_shapes[i]->setID(m_shapes[i]->getID() - 1.f);
				ShapeManipulator::changeIndicesFormat(m_shapes[i], diff);
				ShapeManipulator::extendShapeID(m_shapes[i], m_shapes[i]->getID());
				ShapeManipulator::extendTextureID(m_shapes[i], save_tex_id);
			}

			m_availableShapeID--;
			m_shapesCount--;

			m_samplers.pop_back();
			m_colors.pop_back();
			m_texture->removeID(index);
		}

		void Mesh::popMatrices(const unsigned int& index) {
			m_translationMats.erase(m_translationMats.begin() + index);
			m_rotationMats.erase(m_rotationMats.begin() + index);
		}

		void Mesh::resetDraw() {
			m_shapesDrawn = 0;
		}

		void Mesh::update() {
			for (unsigned int i = 0; i < m_shapesCount; i++) {

				unsigned int currentVerticesSize = m_vertices.size() + m_shapes[i]->getSizeofVertices();
				unsigned int currentIndicesSize = m_indices.size() + m_shapes[i]->getSizeofIndices();

				if (currentVerticesSize <= constants::maxVertexCount && currentIndicesSize <= constants::maxIndexCount) {
					auto beginVert = m_shapes[i]->getVerticesBegin();
					auto endVert = m_shapes[i]->getVerticesEnd();
					auto beginIndices = m_shapes[i]->getIndicesBegin();
					auto endIndices = m_shapes[i]->getIndicesEnd();
					
					m_vertices.insert(m_vertices.end(), beginVert, endVert);
					m_indices.insert(m_indices.end(), beginIndices, endIndices);

					m_texture->bind(m_samplers[i], m_texture->getID(i));

					m_shapesDrawn++;
				}
				else {
					MAR_CORE_ERROR("To much vertices in vector!");
				}
			}		
		}

		void Mesh::clearBuffers() {
			m_vertices.clear();
			m_indices.clear();
		}

		void Mesh::clearMatrices() {
			m_translationMats.clear();
			m_rotationMats.clear();
		}

		void Mesh::pushMatrices(const glm::vec3& center, const glm::vec3& angle) {
			m_translationMats.push_back(glm::translate(glm::mat4(1.0f), center));

			glm::mat4 transform =
				glm::rotate(glm::mat4(1.0f), glm::radians(angle.x), glm::vec3(1.0f, 0.0f, 0.0f))
				* glm::rotate(glm::mat4(1.0f), glm::radians(angle.y), glm::vec3(0.0f, 1.0f, 0.0f))
				* glm::rotate(glm::mat4(1.0f), glm::radians(angle.z), glm::vec3(0.0f, 0.0f, 1.0f));

			m_rotationMats.push_back(transform);
		}


} }