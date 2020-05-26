/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_MATH_MAT_4_H
#define MAR_MATH_MAT_4_H 

#include "../maths.h"

namespace mar {
    namespace maths {

        // I don't know why it must be here, but it somehow works ;)
        struct vec3;

        /*
            column major is used here, cause OpenGL is using it!
            mat4[row + col * 4]
        */
        struct mat4 {
            
            float elements[4 * 4];

            mat4();
            mat4(float diagonal);

            static mat4 identity();
            mat4& multiply(const mat4& other);

            static mat4 orthographic(float left, float right, float top, float bottom, float near, float far);
            static mat4 perspective(float fov, float aspectRatio, float near, float far);

            static mat4 translation(const vec3& trans);
            static mat4 rotation(float angle, const vec3& axis);
            static mat4 scale(const vec3& scal);

            friend mat4 operator*(mat4 left, const mat4& right);
            mat4& operator*=(const mat4& other);
            const float& operator[](unsigned int index) const;

            friend std::ostream& operator<<(std::ostream& stream, const mat4& matrix);
        };

} }

#endif // !MAR_MATH_MAT_4_H

