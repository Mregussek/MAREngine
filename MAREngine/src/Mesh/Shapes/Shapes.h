/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef SHAPES_H
#define SHAPES_H

#include "../../mar.h"

namespace mar {

	class Shapes {
	public:
		glm::vec3 center;
		float* vertices = nullptr;
		unsigned int* indices = nullptr;
		std::vector<unsigned int> layout;

		virtual const glm::vec3& getCenter() const { return {-1.0f, -1.0f, -1.0f}; }
		virtual const unsigned int getStride() const { return -1; }
		virtual const unsigned int getSizeofVertices() const { return -1; };
		virtual const unsigned int getSizeofIndices() const { return -1; };
	};
}

#endif // !SHAPES_H