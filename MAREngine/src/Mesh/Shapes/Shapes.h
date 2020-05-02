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
		float* vertices = nullptr;
		unsigned int* indices = nullptr;
		std::vector<unsigned int> layout;

		virtual const unsigned int getSizeofVertices() const { return -1; };
		virtual const unsigned int getSizeofIndices() const { return -1; };
	};

	class Cube : public Shapes {
	public:
		float vertices[40] = {
			//  front (x, y, z)		// Texture
			-1.0f, -1.0f,  1.0f,	0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f,	1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f,	1.0f, 1.0f,
			-1.0f,  1.0f,  1.0f,	0.0f, 1.0f,
			//  back 
			-1.0f, -1.0, -1.0f,		0.0f, 0.0f,
			 1.0f, -1.0f, -1.0f,	1.0f, 0.0f,
			 1.0f,  1.0, -1.0f,		1.0f, 1.0f,
			-1.0f,  1.0f, -1.0f,	0.0f, 1.0f,
		};

		unsigned int indices[36] = {
			// front	// back
			0, 1, 2,	7, 6, 5,
			2, 3, 0,	5, 4, 7,
			// right	// left
			1, 5, 6,	4, 0, 3,
			6, 2, 1,	3, 7, 4,
			// bottom	// top
			4, 5, 1,	3, 2, 6,
			1, 0, 4,	6, 7, 3
		};

		std::vector<unsigned int> layout = {
			3, 2
		};

		const unsigned int getSizeofVertices() const override { return 40 * sizeof(float); }
		const unsigned int getSizeofIndices() const override { return 36 * sizeof(unsigned int); }
	};

}

#endif // !SHAPES_H