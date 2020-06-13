/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Shapes.h"

namespace mar {
	namespace graphics {


		Shape::Shape(float new_id, std::string new_name, glm::vec3 new_center, glm::vec3 new_angle,
			std::vector<float> new_vertices, std::vector<unsigned int> new_indices,
			std::vector<unsigned int> new_layout)
			: m_id(new_id),
			m_texid(0.0f),
			m_name(new_name),
			m_center(new_center),
			m_angle(new_angle),
			m_verticesVector(new_vertices),
			m_indicesVector(new_indices),
			m_layout(new_layout),
			m_calculatedStride(false),
			m_stride(0)
		{}

		Shape::Shape(const Ref<Shape>& shape)
			: m_id(shape->getID()),
			m_texid(0.0f),
			m_name(shape->getName()),
			m_center(shape->getCenter()),
			m_angle(shape->getAngle()),
			m_verticesVector(shape->getVerticesVector()),
			m_indicesVector(shape->getIndicesVector()),
			m_layout(shape->getLayoutVector()),
			m_calculatedStride(false),
			m_stride(0)
		{}

		void Shape::assignDataFromFile(const std::string& path) {
			objects::ObjectLoader::loadObject(path);

			m_name = objects::ObjectLoader::getName();
			m_verticesVector = objects::ObjectLoader::getVertices();
			m_indicesVector = objects::ObjectLoader::getIndices();
			m_layout = objects::ObjectLoader::getLayout();
			m_center = objects::ObjectLoader::getCenter();
			m_angle = objects::ObjectLoader::getAngle();
			m_id = objects::ObjectLoader::getID();
			m_texid = objects::ObjectLoader::getTextureID();

			MAR_CORE_INFO("Assigned object from ObjectLoader!");
		}

		const unsigned int Shape::getStride() {
			if (!m_calculatedStride) {
				for (auto const& l : m_layout) m_stride += l;
				m_calculatedStride = true;
				return m_stride;
			}

			return m_stride;
		}

		const std::string& Shape::getName() const { return m_name; }

		void Shape:: setName(const std::string& new_name) { m_name = new_name; }

		void Shape::setID(float newID) { m_id = newID; }

		const float Shape::getID() const { return m_id; }

		void Shape::setTextureID(float newID) { m_texid = newID; }

		const float Shape::getTextureID() const { return m_texid; }

		glm::vec3& Shape::getCenter() { return m_center; }

		void Shape::setCenter(const glm::vec3& new_center) { m_center = new_center; }

		glm::vec3& Shape::getAngle() { return m_angle; }

		void Shape::setAngle(const glm::vec3& new_angle) { m_angle = new_angle; }

		const unsigned int Shape::getSizeofVertices() const { return m_verticesVector.size(); }

		const unsigned int Shape::getSizeofIndices() const { return m_indicesVector.size(); }

		const unsigned int& Shape::getMaxValueOfIndices() const { return *std::max_element(m_indicesVector.begin(), m_indicesVector.end()); }

		const unsigned int& Shape::getMinValueOfIndices() const { return *std::min_element(m_indicesVector.begin(), m_indicesVector.end()); }

		void Shape::setVerticesVector(const std::vector<float>& new_vertices) { m_verticesVector = new_vertices; }

		const std::vector<float>& Shape::getVerticesVector() const { return m_verticesVector; }

		float Shape::getVertice(size_t index) const { return m_verticesVector[index]; }

		void Shape::setVertice(size_t index, float new_value) { m_verticesVector[index] = new_value; }

		std::vector<float>::const_iterator Shape::getVerticesBegin() const { return m_verticesVector.begin(); }

		std::vector<float>::const_iterator Shape::getVerticesEnd() const { return m_verticesVector.end(); }

		void Shape::setIndicesVector(const std::vector<unsigned int>& new_indices) { m_indicesVector = new_indices; }

		const std::vector<unsigned int>& Shape::getIndicesVector() const { return m_indicesVector; }

		unsigned int Shape::getIndice(size_t index) const { return m_indicesVector[index]; }

		void Shape::setIndice(size_t index, unsigned int new_value) { m_indicesVector[index] = new_value; }

		std::vector<unsigned int>::const_iterator Shape::getIndicesBegin() const { return m_indicesVector.begin(); }

		std::vector<unsigned int>::const_iterator Shape::getIndicesEnd() const { return m_indicesVector.end(); }

		const std::vector<unsigned int>& Shape::getLayoutVector() const { return m_layout; }

		void Shape::setLayout(const std::vector<unsigned int>& new_layout) { m_layout = new_layout; }

		unsigned int Shape::getLayout(size_t index) const { return m_layout[index]; }

		unsigned int Shape::getLayoutSize() const { return m_layout.size(); }


} }