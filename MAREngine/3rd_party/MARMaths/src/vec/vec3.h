/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_MATH_VEC3_H
#define MAR_MATH_VEC3_H

#include "../maths.h"

namespace mar {
	namespace maths {

		/*
		3-dimensional vector.
		*/
		struct vec3 {

			// -------------------------------
			float x; /* x value of vec3 */
			float y; /* y value of vec3 */
			float z; /* w value of vec3 */
			// -------------------------------

			// Default constructor, creates vec3(0, 0, 0).
			vec3();
			
			/*
			Constructor, that can create vec3 from given 3 floats (l-values).

				\param x - x value, that will be prescribed to vec3(x, y, z)
				\param y - y value, that will be prescribed to vec3(x, y, z)
				\param z - z value, that will be prescribed to vec3(x, y, z)
			*/
			vec3(float x, float y, float z);

			/*
			Addition method of vec3 and float value. 
			vec3(x, y, z) + float = vec3(x + float, y + float, z + float)

				\param f - float value, which will be added
				\return vec3 - returns modified *this
			*/
			vec3& add(float f);

			/*
			Subtraction method of vec3 and float value. 
			vec3(x, y, z) - float = vec3(x - float, y - float, z - float)

				\param f - float value, which will be subtracted
				\return vec3 - returns modified *this
			*/
			vec3& subtract(float f);

			/*
			Multiplication method of vec3 and float value. 
			vec3(x, y, z) * float = vec3(x * float, y * float, z * float)

				\param f - float value, which will be multiplied
				\return vec3 - returns modified *this
			*/
			vec3& multiply(float f);

			/*
			Division method of vec3 and float value. 
			vec3(x, y, z) / float = vec3(x / float, y / float, z / float)

			If user send f as 0, it will return unmodified *this!

				\param f - float value, which will be divided
				\return vec3 - returns modified *this
			*/
			vec3& divide(float f);

			/*
			Addition method of vec3 and vec3.
			v1 + v2 = vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z)

				\param other - second vec3, which will be added to *this
				\return vec3 - computed *this
			*/
			vec3& add(const vec3& other);

			/*
			Subtraction method of vec3 and vec3.
			v1 - v2 = vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z)

				\param other - second vec3, which will be subtracted from *this
				\return vec3 - computed *this
			*/
			vec3& subtract(const vec3& other);

			/*
			Multiplication method of vec3 and vec3.
			v1 * v2 = vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z)

				\param other - second vec3, which will be mutliplied with *this
				\return vec3 - computed *this
			*/
			vec3& multiply(const vec3& other);

			/*
			Division method of vec3 and vec3.
			v1 / v2 = vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z)

			Please make sure, that other vec3 isn't equal to 0 (any of its values). If so,
			method returns unmodified *this.

				\param other - second vec3
				\return vec3 - computed *this
			*/
			vec3& divide(const vec3& other);

			/*
			Computes cross Product of *this and other vec3.

				\param other - vec3
				\return vec3 - result of cross product
			*/
			vec3 cross(const vec3& other);

			/*
			Static method, Computes cross Product of 2 given vec3's.

				\param x - first vec3
				\param y - second vec3
				\return vec3 - result of cross product
			*/
			static vec3 cross(const vec3& x, const vec3& y);

			/*
			Computes dot product of *this and other vec3.

				\param other - vec3
				\return float - calculated dot product
			*/
			float dot(const vec3& other);

			/*
			Static method, which computes dot product of 2 given vec3's.

				\param left - first vec3
				\param right - second vec3
				\return float - calculated dot product
			*/
			static float dot(const vec3& left, const vec3& right);

			/*
			Calculate length / magnitude of a vector.

				\return float - its magnitude
			*/
			float length() const;

			/*
			Computes length of given vector as a paramater (l-value).

				\param v - vec3, which length will be calculated
				\return float - calculated length
			*/
			static float length(vec3& v);

			/*
			Computes normalized vec2. Firstly it calculates length of vector, 
			then it divides every value with length.

			If magnitude is equal to 0, we have debug break.

				\param other - vec3, which will be normalized
				\return vec3 - normalized vec3
			*/
			static vec3 normalize(const vec3& other);

			static const float* value_ptr(const std::vector<vec3>& vec) {
				return &(*vec.data()).x;
			}

			static const float* value_ptr(const vec3& vec) {
				return &vec.x;
			}

			static float* value_ptr_nonconst(vec3& vec) {
				return  &vec.x;
			}

			// -------------------------------------------
			// Overloaded operators, I think they are self-explanatory.
			// -------------------------------------------

			friend vec3 operator+(vec3 left, float right);
			friend vec3 operator-(vec3 left, float right);
			friend vec3 operator*(vec3 left, float right);
			friend vec3 operator/(vec3 left, float right);

			friend vec3 operator+(vec3 left, const vec3& right);
			friend vec3 operator-(vec3 left, const vec3& right);
			friend vec3 operator*(vec3 left, const vec3& right);
			friend vec3 operator/(vec3 left, const vec3& right);

			vec3& operator+=(float other);
			vec3& operator-=(float other);
			vec3& operator*=(float other);
			vec3& operator/=(float other);

			vec3& operator+=(const vec3& other);
			vec3& operator-=(const vec3& other);
			vec3& operator*=(const vec3& other);
			vec3& operator/=(const vec3& other);

			bool operator==(const vec3& other);
			bool operator!=(const vec3& other);

			friend std::ostream& operator<<(std::ostream& stream, const vec3& vector);

		};

	}
}


#endif // !MAR_MATH_VEC3_H
