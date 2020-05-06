/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Cube.h"

namespace mar {
	void Cube::setID(float newID) { 
		id = newID; 
	}

	const float Cube::getID() const {
		return id;
	}

	const glm::vec3 Cube::getCenter() const {
		return center;
	}

	void Cube::setCenter(const glm::vec3& new_center) {
		center = new_center;
	}

	const unsigned int Cube::getStride() {
		if (!_calculatedStride) {
			for (auto const& l : layout) _stride += l;
			_calculatedStride = true;
			return _stride;
		}

		return _stride;
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

	void Cube::setVerticesVector(const std::vector<float>& new_vertices) { 
		verticesVector = new_vertices; 
	}

	const std::vector<float>& Cube::getVerticesVector() const { 
		return verticesVector; 
	}

	float Cube::getVertice(size_t index) const { 
		return verticesVector[index]; 
	}

	void Cube::setVertice(size_t index, float new_value) { 
		verticesVector[index] = new_value; 
	}

	std::vector<float>::const_iterator Cube::getVerticesBegin() const { 
		return verticesVector.begin(); 
	}

	std::vector<float>::const_iterator Cube::getVetricesEnd() const { 
		return verticesVector.end(); 
	}

	void Cube::setIndicesVector(const std::vector<unsigned int>& new_indices) {
		indicesVector = new_indices;
	}

	const std::vector<unsigned int>& Cube::getIndicesVector() const { 
		return indicesVector; 
	}

	unsigned int Cube::getIndice(size_t index) const { 
		return indicesVector[index]; 
	}

	void Cube::setIndice(size_t index, unsigned int new_value) { 
		indicesVector[index] = new_value; 
	}

	std::vector<unsigned int>::const_iterator Cube::getIndicesBegin() const { 
		return indicesVector.begin(); 
	}

	std::vector<unsigned int>::const_iterator Cube::getIndicesEnd() const { 
		return indicesVector.end(); 
	}

	unsigned int Cube::getLayout(size_t index) const { 
		return layout[index]; 
	}

	unsigned int Cube::getLayoutSize() const { 
		return layout.size(); 
	}
}