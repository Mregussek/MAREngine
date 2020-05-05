/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

/*
#include "Shapes.h"

namespace mar {
	void Shapes::setCenter(const glm::vec3& new_center) {
		center = new_center;
	}

	const float Shapes::getID() const {
		return id;
	}

	const glm::vec3 Shapes::getCenter() const {
		return center;
	}

	const unsigned int Shapes::getStride() const {
		unsigned int sum = 0;
		for (auto const& l : layout) sum += l;
		return sum;
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

	void Shapes::changeVerticesIndex(unsigned int index, float new_value) {
		verticesVector[index] = new_value;
	}

	void Shapes::increaseIndice(unsigned int index, unsigned int new_value) {
		indicesVector[index] += new_value;
	}
}

*/