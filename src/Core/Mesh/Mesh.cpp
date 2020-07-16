/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Mesh.h"


namespace mar {
    namespace graphics {

		float Mesh::s_availableTextureID = 1.0f;
		uint32_t Mesh::s_existingInstance = 0;


		Mesh::~Mesh() {
			m_texture->shutdown();

			clearMatrices();
			clearBuffers();

			m_samplers.clear();

			for (auto& s : m_shapes)
				s.reset();

			m_texture.reset();

			m_shapes.clear();

			s_existingInstance--;

			MAR_CORE_INFO("MESH: Mesh has been deleted!");
		}

		void Mesh::create() {
			m_texture = storage::factory->createTexture();
			s_existingInstance++;

			MAR_CORE_INFO("MESH: Mesh has been created!");
		}

		void Mesh::loadScene(Scene* scene, MeshType type) {
			uint32_t shapesInSceneCount = scene->getShapesNumber();
			setMeshType(type);

			switch(m_type) {
			case MeshType::NORMAL:
			case MeshType::CUBEMAPS:
				for (uint32_t i = 0; i < shapesInSceneCount; i++)
					submitShape(scene->getShape(i), scene->getCenter(i), scene->getAngle(i), 
						scene->getScale(i), scene->getTexture(i));
					
				MAR_CORE_INFO("MESH: Scene has been loaded by textures!");

				break;
			case MeshType::OBJECTS:
				for (uint32_t i = 0; i < shapesInSceneCount; i++) {
					auto shape = scene->getShape(i);
					shape->assignDataFromFile(scene->getObjPath(i));

					submitShape(shape, scene->getCenter(i), scene->getAngle(i), 
						scene->getScale(i), scene->getTexture(i));
				}
					
				MAR_CORE_INFO("MESH: Scene has been loaded by objects!");

				break;
			default:
				MAR_CORE_ERROR("MESH: Unsupported mesh type!");
			}
		}

		void Mesh::tryReuseShape(Ref<Shape>& new_shape, const maths::vec3& center, const maths::vec3& angle, const maths::vec3& scale, const char* texture) {
			if (!canPushShape(new_shape)) {
				MAR_CORE_ERROR("MESH: Cannot push more shapes!");
				return;
			}

			if (m_shapes.size() != m_shapesCount) {
				new_shape->setCenter(center);
				new_shape->setAngle(angle);
				new_shape->setScaleVec(scale);
				pushTexture(new_shape, texture);
				pushMatrices(new_shape);
				prepareShape(new_shape);
				MeshCreator::moveShape(m_shapes[m_shapesCount], new_shape);
				pushShape(m_shapes[m_shapesCount]);

				m_shapesCount++;

				MAR_CORE_INFO("MESH: Reusing shape object!");

				return;
			}

			submitShape(new_shape, center, angle, scale, texture);
		}

		void Mesh::submitShape(Ref<Shape>& new_shape, const maths::vec3& center, const maths::vec3& angle, const maths::vec3& scale, const char* texture) {
			if (!canPushShape(new_shape)) {
				MAR_CORE_ERROR("MESH: Cannot push more shapes!");
				return;
			}
			
			new_shape->setCenter(center);
			new_shape->setAngle(angle);
			new_shape->setScaleVec(scale);
			pushTexture(new_shape, texture);
			pushMatrices(new_shape);
			prepareShape(new_shape);
			pushShape(new_shape);

			m_shapes.push_back(new_shape);
			m_shapes[m_shapesCount]->setUsedTexture(texture);
			m_shapesCount++;

			MAR_CORE_INFO("MESH: Added new object to scene!");
		}

		void Mesh::pushTexture(Ref<Shape>& new_shape, const char* texture) {
			MAR_CORE_TRACE("MESH: Trying to push texture");

			// If texture is not equal to empty (if empty we want to load default colors)
			if (std::strcmp(texture, "resources/textures/empty") != 0) { 
				// if texture is not ending with .jpg
				if (!checkEnding(".jpg", texture)) {					
					if (getMeshType() == CUBEMAPS_MESH_TYPE) {
						m_texture->loadCubemap(texture);
					}
					else {
						MAR_CORE_ERROR("MESH: Pushed Cubemap texture to non-cubemap mesh type");
						goto loading_default_colors;
					}
				}
				else {
					if(getMeshType() != CUBEMAPS_MESH_TYPE)
						m_texture->loadTexture(texture);
					else {
						MAR_CORE_ERROR("MESH: Pushed 2D texture to cubemap mesh type!");
						goto loading_default_colors;
					}
				}

				ShapeManipulator::extendTextureID(new_shape, s_availableTextureID);

				m_samplers.push_back((int)s_availableTextureID);
				s_availableTextureID++;
			}
			else {
			loading_default_colors:

				new_shape->setTextureID(0.f);
				m_samplers.push_back(0);
				m_texture->addID(0);
			}

			m_colors.push_back(new_shape->getDefaultColor());
			new_shape->setUsedTexture(texture);
		}

		void Mesh::prepareShape(Ref<Shape>& new_shape) {
			MAR_CORE_TRACE("MESH: Preparing shape");

			ShapeManipulator::extendShapeID(new_shape, m_availableShapeID);
			m_availableShapeID++;

			ShapeManipulator::changeIndicesFormat(new_shape, m_indicesMaxValue);
			m_indicesMaxValue += new_shape->getVertices().size() / new_shape->getStride();
		}

		void Mesh::pushShape(const Ref<Shape>& new_shape) {
			MAR_CORE_TRACE("MESH: Pushing shape");

			auto beginVert = new_shape->getVertices().begin();
			auto endVert = new_shape->getVertices().end();
			auto beginIndices = new_shape->getIndices().begin();
			auto endIndices = new_shape->getIndices().end();

			m_vertices.insert(m_vertices.end(), beginVert, endVert);
			m_indices.insert(m_indices.end(), beginIndices, endIndices);
		}

		void Mesh::pushMatrices(const Ref<Shape>& new_shape) {
			MAR_CORE_TRACE("MESH: Pushing matrix");

			new_shape->setTranslation(
				maths::mat4::translation(new_shape->getCenter())
			);

			new_shape->setRotation(
				maths::mat4::rotation(maths::Trig::toRadians(new_shape->getAngle().x), { 1.0f, 0.0f, 0.0f }) *
				maths::mat4::rotation(maths::Trig::toRadians(new_shape->getAngle().y), { 0.0f, 1.0f, 0.0f }) *
				maths::mat4::rotation(maths::Trig::toRadians(new_shape->getAngle().z), { 0.0f, 0.0f, 1.0f })
			);

			new_shape->setScaleMat(
				maths::mat4::scale(new_shape->getScaleVec())
			);

			m_translationMats.push_back(new_shape->getTranslation());
			m_rotationMats.push_back(new_shape->getRotation());
			m_scaleMats.push_back(new_shape->getScaleMat());
		}

		void Mesh::flushShape(const uint32_t& index) {
			if (m_shapesCount == 1) {
				MAR_CORE_ERROR("MESH: Cannot delete the last shape!");
				return;
			}

			popShape(index);
			popMatrices(index);

			MAR_CORE_INFO("MESH: Deleted object from scene!");
		}

		void Mesh::popShape(const uint32_t& index) {
			MAR_CORE_TRACE("MESH: Trying to pop shape");

			const auto& vec = m_shapes[index]->getIndices();
			uint32_t min_value = *std::min_element(vec.begin(), vec.end());
			uint32_t max_value = *std::max_element(vec.begin(), vec.end());
			int diff = (-1) * (max_value - min_value + 1);

			m_indicesMaxValue -= m_shapes[index]->getVertices().size() / m_shapes[index]->getStride();

			for (uint32_t i = index; i < m_shapesCount - 1; i++) {
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

			clearBuffers();
			updateBuffers();
		}

		void Mesh::popMatrices(const uint32_t& index) {
			MAR_CORE_TRACE("MESH: Popping matrices");

			m_translationMats.erase(m_translationMats.begin() + index);
			m_rotationMats.erase(m_rotationMats.begin() + index);
			m_scaleMats.erase(m_scaleMats.begin() + index);
		}

		void Mesh::update() {
			MAR_CORE_TRACE("MESH: update");

			for (uint32_t i = 0; i < m_shapesCount; i++)
				m_texture->bind(m_samplers[i], m_texture->getID(i));
		}

		void Mesh::clearBuffers() {
			MAR_CORE_TRACE("MESH: Clearing buffers");

			m_vertices.clear();
			m_indices.clear();
		}

		void Mesh::updateBuffers() {
			MAR_CORE_TRACE("MESH: Updating buffers");

			for (uint32_t i = 0; i < m_shapesCount; i++) {
				auto beginVert = m_shapes[i]->getVertices().begin();
				auto endVert = m_shapes[i]->getVertices().end();
				auto beginIndices = m_shapes[i]->getIndices().begin();
				auto endIndices = m_shapes[i]->getIndices().end();

				m_vertices.insert(m_vertices.end(), beginVert, endVert);
				m_indices.insert(m_indices.end(), beginIndices, endIndices);
			}
		}

		void Mesh::clearMatrices() {
			MAR_CORE_TRACE("MESH: Clearing matrices");

			m_translationMats.clear();
			m_rotationMats.clear();
			m_scaleMats.clear();
		}

		bool Mesh::canPushShape(const Ref<Shape>& new_shape) {
			uint32_t currentVerticesSize = m_vertices.size() + new_shape->getVertices().size();
			uint32_t currentIndicesSize = m_indices.size() + new_shape->getIndices().size();

			if (currentVerticesSize >= constants::maxVertexCount) {
				MAR_CORE_ERROR("MESH: To much vertices in vector!");
				return false;
			}

			if (currentIndicesSize >= constants::maxIndexCount) {
				MAR_CORE_ERROR("MESH: To much indices in vector!");
				return false;
			}

			if (m_shapesCount == constants::maxObjectsInScene - 1) {
				MAR_CORE_ERROR("MESH: Cannot push more objects!");
				return false;
			}

			return true;
		}

		bool Mesh::checkEnding(const char* withwhat, const char* what) {
			int l1 = strlen(withwhat);
			int l2 = strlen(what);

			if (l1 > l2)
				return false;

			return std::strcmp(withwhat, what + (l2 - l1)) == 0;
		}


} }