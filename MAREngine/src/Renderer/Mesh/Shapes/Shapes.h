/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef SHAPES_H
#define SHAPES_H

#include "../../../mar.h"

namespace mar {

	struct Vertex {
		float position[3];
		float texCoord[2];
		float texId;
	};

	class Shapes : std::enable_shared_from_this<Shapes> {
	public:
		Shapes(float i, glm::vec3 ctr, std::vector<float> vert, std::vector<unsigned int> indi, std::vector<unsigned int> lay)
			: id(i),
			center(ctr),
			verticesVector(vert),
			indicesVector(indi),
			layout(lay)
		{}

		float id;
		glm::vec3 center;
		std::vector<float> verticesVector;
		std::vector<unsigned int> indicesVector;
		std::vector<unsigned int> layout;

		virtual void setID(float newID) = 0;
		virtual void setCenter(const glm::vec3& new_center) = 0;
		virtual const float getID() const = 0;
		virtual const glm::vec3 getCenter() const = 0;
		virtual const unsigned int getStride() const = 0;
		virtual const unsigned int getSizeofVertices() const = 0;
		virtual const unsigned int getSizeofIndices() const = 0;
		virtual const unsigned int getMaxValueOfIndices() const = 0;
		virtual void changeVerticesIndex(unsigned int index, float new_value) = 0;
		virtual void increaseIndice(unsigned int index, unsigned int new_value) = 0;
	};
}

#endif // !SHAPES_H