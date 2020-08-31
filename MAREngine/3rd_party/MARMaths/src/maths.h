/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_MATH_H
#define MAR_MATH_H

#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath>

#include <vector>

#include "trig/trigonometric.h"

#include "math_func/Basic_Math.h"

#include "vec/vec2.h"
#include "vec/vec3.h"
#include "vec/vec4.h"

#include "mat/mat4.h"

#define MAR_MATH_PI M_PI

namespace mar { 
    namespace maths {

        struct Trig;

		struct vec2;
		struct vec3;
		struct vec4;

        struct mat4;

		struct basic;


} }


#endif // !MAR_MATH_H