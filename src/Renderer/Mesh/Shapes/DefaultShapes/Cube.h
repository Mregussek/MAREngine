/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef CUBE_H
#define CUBE_H
	
#include "../../../../mar.h"
#include "../Shapes.h"

/*		
			(-1.0f, 1.0f, -1.0f)[7]							(1.0f,  1.0f, -1.0f)[6]
						   x-----------------------------------x									
						  /	|								  /|
						 /	|								 / |	
(-1.0f, 1.0f, 1.0f)[3]	/	|		(1.0f,  1.0f, 1.0f)[2]	/  |	
						x----------------------------------x   |
						|	|							   |   |
						|	|							   |   |
						|	|							   |   |
						|	|							   |   |
						|	|							   |   |
						|	|							   |   |
						|	|							   |   |
						|	| (-1.0f, -1.0f, -1.0f)[4]	   |   |
						|	x------------------------------|---x	(1.0f, -1.0f, -1.0f)[5]					      
						|  /							   |  /	
						| /								   | /	
						|/								   |/	
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


		//! Cube
		/*!
			Create new Shape with overloaded class.
		*/
		class Cube : public Shape {
		public:
			Cube()
				: Shape(
					/* ID */
					0.0f,
					/* NAME */
					"Cube",
					/* CENTER */
					{ 0.0f, 0.0f, 0.0f },
					/* ANGLE */
					{ 0.0f, 0.0f, 0.0f },
					/* VERTICES */
				{
					//  front (x, y, z)		// BasicColors			// LightNormal			// Texture		// Texture Index	// ShapeIndex
					-1.0f, -1.0f,  1.0f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,		0.0f,				0.0f, // 0
					 1.0f, -1.0f,  1.0f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f,		0.0f,				0.0f, // 1
					 1.0f,  1.0f,  1.0f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f,				0.0f, // 2
					-1.0f,  1.0f,  1.0f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 1.0f,		0.0f,				0.0f, // 3
					//  back 										
					-1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,		0.0f,				0.0f, // 4
					 1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f,		0.0f,				0.0f, // 5
					 1.0f,  1.0f, -1.0f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f,				0.0f, // 6
					-1.0f,  1.0f, -1.0f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 1.0f,		0.0f,				0.0f  // 7
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
					3, 3, 3, 2, 1, 1
				})
			{ }
		};


} }

#endif // !CUBE_H