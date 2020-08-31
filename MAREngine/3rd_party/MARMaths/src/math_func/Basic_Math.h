/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_MATH_BASIC_MATH_H
#define MAR_MATH_BASIC_MATH_H

#include "../maths.h"
#include "../mat/mat4.h"
#include "../vec/vec3.h"


namespace mar {
	namespace maths {


		/*
		basic structure defines basic mathematical methods.
		*/
		struct basic {
		
			/*
			Calculates square root of given value.

				\param val - value, of which square root will be computed/
				\return float - calculated sqrt(val).
			*/
			static float square(float val) {
				return sqrt(val);
			}

		};


} }

#endif // !MAR_MATH_BASIC_MATH_H
