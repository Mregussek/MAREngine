/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef PYRAMID_H
#define PYRAMID_H

#include "../../../mar.h"
#include "Shapes.h"

namespace mar {

	class Pyramid : public Shapes {
	public:
		Pyramid()
			: Shapes(
				/* ID */
				0.0f,
				/* NAME */
				"Pyramid",
				/* CENTER */
				{ 0.0f, 0.0f, 0.0f },
				/* ANGLE */
				{ 0.0f, 0.0f, 0.0f },
				/* VERTICES */
				{
					// (x, y, z)			// LightNormal			// TextureCoords	// TexIndex
					-1.0f, -1.0f,  1.0f,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f,			0.0f,
					 1.0f, -1.0f,  1.0f,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f,			0.0f,
					 1.0f, -1.0f, -1.0f,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f,			0.0f,
					-1.0f, -1.0f, -1.0f,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f,			0.0f,
					 0.0f,  1.0f,  0.0f,	0.0f, 0.0f, 1.0f,		0.5f, 0.5f,			0.0f
				},
				/* INDICES */
				{
					0, 1, 2,	2, 3, 0, // fundamental quad
					0, 1, 4,	1, 2, 4, // side triangles
					2, 3, 4,	3, 0, 4
				},
				/* LAYOUT */
				{
					3, 3, 2, 1
				})
		{ }
	};

}

#endif // !PYRAMID_H