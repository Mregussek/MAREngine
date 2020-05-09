/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef SURFACE_H
#define SURFACE_H

#include "../../../mar.h"
#include "Shapes.h"

namespace mar {

	class Surface : public Shapes {
	public:
		Surface()
			: Shapes(
				/* ID */
				0.0f,
				/* NAME */
				"Surface",
				/* CENTER */
				{ 0.0f, 0.0f, 0.0f },
				/* ANGLE */
				{ 0.0f, 0.0f, 0.0f },
				/* VERTICES */
				{
					// (x, y, z)			// LightNormal			// TextureCoords	// TexIndex
					-15.0f, -1.0f,  15.0f,	0.0f, 0.0f, -1.0f,		0.0f, 0.0f,			0.0f, // 0
					 15.0f, -1.0f,  15.0f,	0.0f, 0.0f, -1.0f,		0.0f, 1.0f,			0.0f, // 1
					 15.0f, -1.0f, -15.0f,	0.0f, 0.0f, -1.0f,		1.0f, 1.0f,			0.0f, // 2
					-15.0f, -1.0f, -15.0f,	0.0f, 0.0f, -1.0f,		1.0f, 0.0f,			0.0f  // 3
				},
				/* INDICES */
				{
					0, 1, 2, // first triangle
					2, 3, 0  // second triangle
				},
				/* LAYOUT */
				{
					3, 3, 2, 1
				})
		{ }
	};

}

#endif // !SURFACE_H