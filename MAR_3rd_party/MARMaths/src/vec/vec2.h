/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_MATH_VEC2_H
#define MAR_MATH_VEC2_H

#include "../maths.h"

namespace mar { 
	namespace maths {

		struct vec2 {

			float x;
			float y;

			vec2();
			vec2(const float& x, const float& y);
			vec2(const float&& x, const float&& y);

			vec2& add(const vec2& other);
			vec2& subtract(const vec2& other);
			vec2& multiply(const vec2& other);
			vec2& divide(const vec2& other);

			friend vec2 operator+(vec2 left, const vec2& right);
			friend vec2 operator-(vec2 left, const vec2& right);
			friend vec2 operator*(vec2 left, const vec2& right);
			friend vec2 operator/(vec2 left, const vec2& right);

			vec2& operator+=(const vec2& other);
			vec2& operator-=(const vec2& other);
			vec2& operator*=(const vec2& other);
			vec2& operator/=(const vec2& other);

			bool operator==(const vec2& other);
			bool operator!=(const vec2& other);

			friend std::ostream& operator<<(std::ostream& stream, const vec2& vector);

		};

} }


#endif // !MAR_MATH_VEC2_H
