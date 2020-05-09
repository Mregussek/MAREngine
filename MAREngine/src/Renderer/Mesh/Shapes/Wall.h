/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef WALL_H
#define WALL_H

#include "../../../mar.h"
#include "Shapes.h"

namespace mar {

	class Wall : public Shapes {
	public:
		Wall()
			: Shapes(
				/* ID */
				0.0f,
				/* NAME */
				"Wall",
				/* CENTER */
				{ 0.0f, 0.0f, 0.0f },
				/* ANGLE */
				{ 0.0f, 0.0f, 0.0f },
				/* VERTICES */
				{
					//  front (x, y, z)		// LightNormal			// Texture		// Texture Index
					-0.2f, -1.0f,  10.0f,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f,		0.0f, // 0
					 0.2f, -1.0f,  10.0f,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f,		0.0f, // 1
					 0.2f,  5.0f,  10.0f,	0.0f, 0.0f, 1.0f,		1.0f, 0.0f,		0.0f, // 2
					-0.2f,  5.0f,  10.0f,	0.0f, 0.0f, 1.0f,		1.0f, 0.0f,		0.0f, // 3
					//  back 									
					-0.2f, -1.0f, -10.0f,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f,		0.0f, // 4
					 0.2f, -1.0f, -10.0f,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f,		0.0f, // 5
					 0.2f,  5.0f, -10.0f,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, // 6
					-0.2f,  5.0f, -10.0f,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f  // 7
				},
				/* INDICES */
				{
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
				/* LAYOUT */
				{
					3, 3, 2, 1
				})
		{ }
	};

}

#endif // !WALL_H