/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef SURFACE_H
#define SURFACE_H

#include "../../../../mar.h"
#include "../Shapes.h"

 /*

		(-15.0f, -1.0f, -15.0f)[3] 
						  	x----------------------------------x	(15.0f, -1.0f, -15.0f)[2]
						    /							      /
						   /								 /
						  /								    /
						 x----------------------------------x
 (-15.0f, -1.0f,  15.0f)[0]									(15.0f, -1.0f,  15.0f)[1]

 Legend: (x, y, z)[indice]

 The center of this object is x = y = z = 0.0f;
 Also the object is not rotated, so in that case angles on three axis are equal to 0.0f;
 Layout is 3, 2, 1;
	 first number is 3 because, we have three position coordinates (x, y, z)
	 second number is 2 cause, there is two texture coordinates (x, y)
	 third number is 1, figure cannot have more than 2 indexes (not in MAREngine :D)
 */

namespace mar {

	//! Surface
	/*!
		Create new Shape with overloaded class.
	*/
	class Surface : public Shape {
	public:
		Surface()
			: Shape(
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
					// (x, y, z)				// BasicColors		// LightNormal			// TextureCoords	// TexIndex
					-15.0f, -1.0f,  15.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f,			0.0f, // 0
					 15.0f, -1.0f,  15.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f,			0.0f, // 1
					 15.0f, -1.0f, -15.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f,			0.0f, // 2
					-15.0f, -1.0f, -15.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,		1.0f, 0.0f,			0.0f  // 3
				},
				/* INDICES */
				{
					0, 1, 2, // first triangle
					2, 3, 0  // second triangle
				},
				/* LAYOUT */
				{
					3, 3, 3, 2, 1
				})
		{ }
	};

}

#endif // !SURFACE_H