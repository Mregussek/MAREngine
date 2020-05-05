/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef CUBE_H
#define CUBE_H

#include "../../../mar.h"
#include "Shapes.h"

namespace mar {

	class Cube : public Shapes, std::enable_shared_from_this<Cube> {
	public:
		Cube() : Shapes(
			0.0f, // ID 
			glm::vec3(0.0f, 0.0f, 0.0f), // CENTER

		{		// VERTICES IN CONSTRUCTOR
				//  front (x, y, z)		// Texture		// Texture Index
				-1.0f, -1.0f, 1.0f,		 0.0f, 0.0f,	0.0f,
				 1.0f, -1.0f, 1.0f,		 1.0f, 0.0f,	0.0f,
				 1.0f,  1.0f, 1.0f,		 1.0f, 1.0f,	0.0f,
				-1.0f,  1.0f, 1.0f,		 0.0f, 1.0f,	0.0f,
				//  back 								
				-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f,
				 1.0f, -1.0f, -1.0f,	1.0f, 0.0f,		0.0f,
				 1.0f,  1.0f, -1.0f,	1.0f, 1.0f,		0.0f,
				-1.0f,  1.0f, -1.0f,	0.0f, 1.0f,		0.0f
		},
		{		// INDICES IN CONSTRUCTOR
				// front	// back
				0, 1, 2,	7, 6, 5,
				2, 3, 0,	5, 4, 7,
				// right	// left
				1, 5, 6,	4, 0, 3,
				6, 2, 1,	3, 7, 4,
				// bottom	// top
				4, 5, 1,	3, 2, 6,
				1, 0, 4,	6, 7, 3
		},
		{		// LAYOUT IN CONSTRUCTOR
				3, 2, 1
		})
		{	// BODY OF CONSTRUCTOR
			
			id = 0.0f;

			center = glm::vec3(0.0f, 0.0f, 0.0f);

			verticesVector = {
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

			indicesVector = {
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

			layout = {
				3, 2, 1
			};
			
		}

		virtual void setCenter(const glm::vec3& new_center) override;
		virtual void setID(float newID) override { id = newID; }
		virtual const float getID() const override;
		virtual const glm::vec3 getCenter() const override;
		virtual const unsigned int getStride() const override;
		virtual const unsigned int getSizeofVertices() const override;
		virtual const unsigned int getSizeofIndices() const override;
		virtual const unsigned int getMaxValueOfIndices() const override;
		virtual void changeVerticesIndex(unsigned int index, float new_value) override;
		virtual void increaseIndice(unsigned int index, unsigned int new_value) override;

		float id;
		glm::vec3 center;
		std::vector<float> verticesVector;
		std::vector<unsigned int> indicesVector;
		std::vector<unsigned int> layout;
	};

}

#endif // !CUBE_H