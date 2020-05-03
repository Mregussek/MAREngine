/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef CUBE_H
#define CUBE_H

#include "../../mar.h"
#include "Shapes.h"

namespace mar {

	class Cube : public Shapes {
	public:
		Cube() = default;

		void prescribeCenter(const glm::vec3& new_center);

		const glm::vec3 getCenter() const override;
		const unsigned int getStride() const override;
		const unsigned int getSizeofVertices() const override;
		const unsigned int getSizeofIndices() const override;
		const unsigned int getMaxValueOfIndices() const override;

		glm::vec3 center{ 0.0f, 0.0f, 0.0f };

		std::vector<float> verticesVector = {
			//  front (x, y, z)		// Texture		// Texture Index
			-1.0f, -1.0f,  1.0f,	0.0f, 0.0f,		0.0f,
			 1.0f, -1.0f,  1.0f,	1.0f, 0.0f,		0.0f,
			 1.0f,  1.0f,  1.0f,	1.0f, 1.0f,		0.0f,
			-1.0f,  1.0f,  1.0f,	0.0f, 1.0f,		0.0f,
			//  back 								
			-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f,
			 1.0f, -1.0f, -1.0f,	1.0f, 0.0f,		0.0f,
			 1.0f,  1.0f, -1.0f,	1.0f, 1.0f,		0.0f,
			-1.0f,  1.0f, -1.0f,	0.0f, 1.0f,		0.0f
		};

		std::vector<unsigned int> indicesVector = {
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
			3, 2, 1
		};
	};

}

#endif // !CUBE_H