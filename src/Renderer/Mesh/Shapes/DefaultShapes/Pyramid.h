/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef PYRAMID_H
#define PYRAMID_H

#include "../../../../mar.h"
#include "../Shapes.h"

 /*											(0.0f,  1.0f,  0.0f)[5]
											  x
								 			/	\	
										/    /	\	\		
									/	  /		  \		\
								/	    /			\		\
(-1.0f, -1.0f, -1.0f)[4]  	 x-------/----------------\---------x	(1.0f, -1.0f, -1.0f)[5]
						    / 	  /						\      /
						   /   /						  \	  /
						  /	/							    \/
						 x----------------------------------x
 (-1.0f, -1.0f, 1.0f)[0]									(1.0f, -1.0f, 1.0f)[1]

 Legend: (x, y, z)[indice]

 The center of this object is x = y = z = 0.0f;
 Also the object is not rotated, so in that case angles on three axis are equal to 0.0f;
 Layout is 3, 2, 1;
	 first number is 3 because, we have three position coordinates (x, y, z)
	 second number is 2 cause, there is two texture coordinates (x, y)
	 third number is 1, figure cannot have more than 2 indexes (not in MAREngine :D)
 */

namespace mar {
	namespace graphics {


		//! Pyramid
		/*!
			Create new Shape with overloaded class.
		*/
		class Pyramid : public Shape {
		public:
			Pyramid()
				: Shape(
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
					// (x, y, z)			// BasicColors			// LightNormal			// TextureCoords	// TexIndex	// ShapeIndex
					-1.0f, -1.0f,  1.0f,	1.0f, 0.0f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,			0.0f,		0.0f,
					 1.0f, -1.0f,  1.0f,	1.0f, 0.0f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 1.0f,			0.0f,		0.0f,
					 1.0f, -1.0f, -1.0f,	1.0f, 0.0f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,			0.0f,		0.0f,
					-1.0f, -1.0f, -1.0f,	1.0f, 0.0f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 1.0f,			0.0f,		0.0f,
					 0.0f,  1.0f,  0.0f,	1.0f, 0.0f, 0.0f,		0.0f, 0.0f, 1.0f,		0.5f, 0.5f,			0.0f,		0.0f
				},
					/* INDICES */
				{
					0, 1, 2,	2, 3, 0, // fundamental quad
					0, 1, 4,	1, 2, 4, // side triangles
					2, 3, 4,	3, 0, 4
				},
					/* LAYOUT */
				{
					3, 3, 3, 2, 1, 1
				})
			{ }
		};


} }

#endif // !PYRAMID_H