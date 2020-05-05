/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Cube.h"

namespace mar {
	void Cube::setCenter(const glm::vec3& new_center) {
		center = new_center;
	}

	const float Cube::getID() const {
		return id;
	}

	const glm::vec3 Cube::getCenter() const {
		return center;
	}

	const unsigned int Cube::getStride() const {
		unsigned int sum = 0;
		for (auto const& l : layout) sum += l;
		return sum;
	}

	const unsigned int Cube::getSizeofVertices() const { 
		return verticesVector.size(); 
	}

	const unsigned int Cube::getSizeofIndices() const { 
		return indicesVector.size(); 
	}

	const unsigned int Cube::getMaxValueOfIndices() const {
		return *std::max_element(indicesVector.begin(), indicesVector.end()); 
	}

	
	void Cube::changeVerticesIndex(unsigned int index, float new_value) {
		verticesVector[index] = new_value;
	}

	void Cube::increaseIndice(unsigned int index, unsigned int new_value) {
		indicesVector[index] += new_value;
	}
	
}