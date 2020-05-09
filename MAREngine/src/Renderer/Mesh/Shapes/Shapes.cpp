/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Shapes.h"

namespace mar {
	void Shapes::setID(float newID) {
		id = newID;
	}

	const float Shapes::getID() const {
		return id;
	}

	const glm::vec3 Shapes::getCenter() const {
		return center;
	}

	void Shapes::setCenter(const glm::vec3& new_center) {
		center = new_center;
	}

	const glm::vec3 Shapes::getAngle() const {
		return angle;
	}

	void Shapes::setAngle(const glm::vec3& new_angle) {
		center = new_angle;
	}

	const unsigned int Shapes::getStride() {
		if (!_calculatedStride) {
			for (auto const& l : layout) _stride += l;
			_calculatedStride = true;
			return _stride;
		}

		return _stride;
	}

	const unsigned int Shapes::getSizeofVertices() const {
		return verticesVector.size();
	}

	const unsigned int Shapes::getSizeofIndices() const {
		return indicesVector.size();
	}

	const unsigned int Shapes::getMaxValueOfIndices() const {
		return *std::max_element(indicesVector.begin(), indicesVector.end());
	}

	void Shapes::setVerticesVector(const std::vector<float>& new_vertices) {
		verticesVector = new_vertices;
	}

	const std::vector<float>& Shapes::getVerticesVector() const {
		return verticesVector;
	}

	float Shapes::getVertice(size_t index) const {
		return verticesVector[index];
	}

	void Shapes::setVertice(size_t index, float new_value) {
		verticesVector[index] = new_value;
	}

	std::vector<float>::const_iterator Shapes::getVerticesBegin() const {
		return verticesVector.begin();
	}

	std::vector<float>::const_iterator Shapes::getVerticesEnd() const {
		return verticesVector.end();
	}

	void Shapes::setIndicesVector(const std::vector<unsigned int>& new_indices) {
		indicesVector = new_indices;
	}

	const std::vector<unsigned int>& Shapes::getIndicesVector() const {
		return indicesVector;
	}

	unsigned int Shapes::getIndice(size_t index) const {
		return indicesVector[index];
	}

	void Shapes::setIndice(size_t index, unsigned int new_value) {
		indicesVector[index] = new_value;
	}

	std::vector<unsigned int>::const_iterator Shapes::getIndicesBegin() const {
		return indicesVector.begin();
	}

	std::vector<unsigned int>::const_iterator Shapes::getIndicesEnd() const {
		return indicesVector.end();
	}

	unsigned int Shapes::getLayout(size_t index) const {
		return layout[index];
	}

	unsigned int Shapes::getLayoutSize() const {
		return layout.size();
	}
}