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

	class Shapes {
	public:
		glm::vec3 center = {0.0f, 0.0f, 0.0f};
		float* vertices = nullptr;
		unsigned int* indices = nullptr;
		std::vector<unsigned int> layout;

		virtual void setID(float newID) { }

		virtual const float getID() const { return -1; }
		virtual const glm::vec3 getCenter() const { return {-1.0f, -1.0f, -1.0f}; }
		virtual const unsigned int getStride() const { return -1; }
		virtual const unsigned int getMaxValueOfIndices() const { return -1; }
		virtual const unsigned int getSizeofVertices() const { return -1; };
		virtual const unsigned int getSizeofIndices() const { return -1; };
	};
}

#endif // !SHAPES_H