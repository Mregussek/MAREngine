/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_MATH_FUNCTIONS_H
#define MAR_MATH_FUNCTIONS_H

#include "maths.h"

namespace mar {
    namespace maths {

        struct Angles {
            
            static float toRadians(float degrees) {
                return degrees * ((float) M_PI / 180.0f);
            }

            static float toDegrees(float radians) {
                return 0.0f;
            }

        };

    }
}


#endif // !MAR_MATH_FUNCTIONS_H