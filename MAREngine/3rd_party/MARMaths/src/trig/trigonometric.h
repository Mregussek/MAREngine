/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_MATH_TRIGONOMETRIC_H
#define MAR_MATH_TRIGONOMETRIC_H

#include "../maths.h"

namespace mar {
    namespace maths {

        /*
        Trig is a structure, that gives some trigonometric functions, such as sine, 
        tangent calculations (and many others).
        */
        struct Trig {

            static float toRadians(float degrees) {
                return degrees * 0.01745329251f;
            }

            static float toDegrees(float radians) {
                return radians * 57.29577951308f;
            }

            static float sine(float radians) {
                return sin(radians);
            }

            static float cosine(float radians) {
                return cos(radians);
            }

            static float tangent(float radians) {
                return tan(radians);
            }

            static float arcsine(float radians) {
                return asin(radians);
            }

            static float arccosine(float radians) {
                return acos(radians);
            }

            static float arctangent(float radians) {
                return atan(radians);
            }

            static float h_sine(float radians) {
                return sinh(radians);
            }

            static float h_cosine(float radians) {
                return cosh(radians);
            }

            static float h_tangent(float radians) {
                return tanh(radians);
            }

            static float h_arcsine(float radians) {
                return asinh(radians);
            }

            static float h_arccosine(float radians) {
                return acosh(radians);
            }

            static float h_arctangent(float radians) {
                return atanh(radians);
            }

        };

    }
}


#endif // !MAR_MATH_TRIGONOMETRIC_H
