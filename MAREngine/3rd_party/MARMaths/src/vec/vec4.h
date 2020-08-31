/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_MATH_VEC4_H
#define MAR_MATH_VEC4_H

#include "../maths.h"

namespace mar { 
	namespace maths {

		// Linker problems
		struct vec3;

		/*
		4-dimensional vector.
		*/
		struct vec4 {
			
			// -------------------------------
			float x; /* x value of vec4 */
			float y; /* y value of vec4 */
			float z; /* z value of vec4 */
			float w; /* w value of vec4 */
			// ------------------------------

			// Default constructor, creates vec4(0, 0, 0, 0).
			vec4();

			/* 
			Constructor, that expands vec3 to vec4 with given w parameter.

				\param v - vec3, which will be expanded
				\param w - value, which is needed to create fourth dimension
			*/
			vec4(const vec3& v, float w);
			
			/*
			Constructor, that can create vec4 from given 4 floats (l-values).

				\param x - x value, that will be prescribed to vec4(x, y, z, w)
				\param y - y value, that will be prescribed to vec4(x, y, z, w)
				\param z - z value, that will be prescribed to vec4(x, y, z, w)
				\param w - w value, that will be prescribed to vec4(x, y, z, w)
			*/
			vec4(float x, float y, float z, float w);

			/*
			Addition method of vec4 and float value. 
			vec4(x, y, z, w) + float = vec4(x + float, y + float, z + float, w + float)

				\param f - float value, which will be added
				\return vec4 - returns modified *this
			*/
			vec4& add(float f);

			/*
			Subtraction method of vec4 and float value. 
			vec4(x, y, z, w) - float = vec4(x - float, y - float, z - float, w - float)

				\param f - float value, which will be subtracted
				\return vec4 - returns modified *this
			*/
			vec4& subtract(float f);

			/*
			Multiplication method of vec4 and float value. 
			vec4(x, y, z, w) * float = vec4(x * float, y * float, z * float, w * float)

				\param f - float value, which will be multiplied
				\return vec4 - returns modified *this
			*/
			vec4& multiply(float f);

			/*
			Division method of vec4 and float value. 
			vec4(x, y, z, w) / float = vec4(x / float, y / float, z / float, w / float)

			If user send f as 0, it will return unmodified *this!

				\param f - float value, which will be divided
				\return vec4 - returns modified *this
			*/
			vec4& divide(float f);

			/*
			Addition method of vec4 and vec4.
			v1 + v2 = vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w)

				\param other - second vec4, which will be added to *this
				\return vec4 - computed *this
			*/
			vec4& add(const vec4& other);

			/*
			Subtraction method of vec4 and vec4.
			v1 - v2 = vec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w)

				\param other - second vec4, which will be subtracted from *this
				\return vec4 - computed *this
			*/
			vec4& subtract(const vec4& other);

			/*
			Multiplication method of vec4 and vec4.
			v1 * v2 = vec4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w)

				\param other - second vec4, which will be mutliplied with *this
				\return vec4 - computed *this
			*/
			vec4& multiply(const vec4& other);

			/*
			Division method of vec4 and vec4.
			v1 / v2 = vec4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w)

			Please make sure, that other vec4 isn't equal to 0 (any of its values). If so,
			method returns unmodified *this.

				\param other - second vec4
				\return vec4 - computed *this
			*/
			vec4& divide(const vec4& other);

			/*
			Computes dot product of *this and other vec4.

				\param other - vec4
				\return float - calculated dot product
			*/
			float dot(const vec4& other);

			/*
			Static method, which computes dot product of 2 given vec4's.

				\param left - first vec4
				\param right - second vec4
				\return float - calculated dot product
			*/
			static float dot(const vec4& left, const vec4& right);

			/*
			Calculate length / magnitude of a vector.

				\return float - its magnitude
			*/
			float length() const;

			/*
			Computes length of given vector as a paramater (l-value).

				\param v - vec4, which length will be calculated
				\return float - calculated length
			*/
			static float length(const vec4& v);

			/*
			Computes normalized vec4. Firstly it calculates length of vector, 
			then it divides every value with length.

			If magnitude is equal to 0, we have debug break.

				\param other - vec4, which will be normalized
				\return vec4 - normalized vec4
			*/
			static vec4 normalize(const vec4& other);

			// -------------------------------------------
			// Overloaded operators, I think they are self-explanatory.
			// -------------------------------------------

			friend vec4 operator+(vec4 left, float right);
			friend vec4 operator-(vec4 left, float right);
			friend vec4 operator*(vec4 left, float right);
			friend vec4 operator/(vec4 left, float right);

			friend vec4 operator+(vec4 left, const vec4& right);
			friend vec4 operator-(vec4 left, const vec4& right);
			friend vec4 operator*(vec4 left, const vec4& right);
			friend vec4 operator/(vec4 left, const vec4& right);

			vec4& operator+=(float other);
			vec4& operator-=(float other);
			vec4& operator*=(float other);
			vec4& operator/=(float other);

			vec4& operator+=(const vec4& other);
			vec4& operator-=(const vec4& other);
			vec4& operator*=(const vec4& other);
			vec4& operator/=(const vec4& other);

			const float& operator[](unsigned int index) const;
			float& operator[](unsigned int index);

			bool operator==(const vec4& other);
			bool operator!=(const vec4& other);

			friend std::ostream& operator<<(std::ostream& stream, const vec4& vector);

		};

	}
}


#endif // !MAR_MATH_VEC4_H
