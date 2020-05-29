/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Shapes.h"

namespace mar {
	namespace graphics {


		const std::string& Shape::getName() const {
			return name;
		}

		void Shape::setID(float newID) {
			id = newID;
		}

		const float Shape::getID() const {
			return id;
		}

		void Shape::setTextureID(float newID) {
			textureId = newID;
		}

		const float Shape::getTextureID() const {
			return textureId;
		}

		const glm::vec3 Shape::getCenter() const {
			return center;
		}

		void Shape::setCenter(const glm::vec3& new_center) {
			center = new_center;
		}

		const glm::vec3 Shape::getAngle() const {
			return angle;
		}

		void Shape::setAngle(const glm::vec3& new_angle) {
			center = new_angle;
		}

		const unsigned int Shape::getStride() {
			if (!_calculatedStride) {
				for (auto const& l : layout) _stride += l;
				_calculatedStride = true;
				return _stride;
			}

			return _stride;
		}

		const unsigned int Shape::getSizeofVertices() const {
			return verticesVector.size();
		}

		const unsigned int Shape::getSizeofIndices() const {
			return indicesVector.size();
		}

		const unsigned int Shape::getMaxValueOfIndices() const {
			return *std::max_element(indicesVector.begin(), indicesVector.end());
		}

		void Shape::setVerticesVector(const std::vector<float>& new_vertices) {
			verticesVector = new_vertices;
		}

		const std::vector<float>& Shape::getVerticesVector() const {
			return verticesVector;
		}

		float Shape::getVertice(size_t index) const {
			return verticesVector[index];
		}

		void Shape::setVertice(size_t index, float new_value) {
			verticesVector[index] = new_value;
		}

		std::vector<float>::const_iterator Shape::getVerticesBegin() const {
			return verticesVector.begin();
		}

		std::vector<float>::const_iterator Shape::getVerticesEnd() const {
			return verticesVector.end();
		}

		void Shape::setIndicesVector(const std::vector<unsigned int>& new_indices) {
			indicesVector = new_indices;
		}

		const std::vector<unsigned int>& Shape::getIndicesVector() const {
			return indicesVector;
		}

		unsigned int Shape::getIndice(size_t index) const {
			return indicesVector[index];
		}

		void Shape::setIndice(size_t index, unsigned int new_value) {
			indicesVector[index] = new_value;
		}

		std::vector<unsigned int>::const_iterator Shape::getIndicesBegin() const {
			return indicesVector.begin();
		}

		std::vector<unsigned int>::const_iterator Shape::getIndicesEnd() const {
			return indicesVector.end();
		}

		const std::vector<unsigned int>& Shape::getLayoutVector() const {
			return layout;
		}

		unsigned int Shape::getLayout(size_t index) const {
			return layout[index];
		}

		unsigned int Shape::getLayoutSize() const {
			return layout.size();
		}


} }