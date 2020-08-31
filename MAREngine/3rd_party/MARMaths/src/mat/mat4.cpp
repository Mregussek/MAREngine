/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "mat4.h"

namespace mar {
    namespace maths {

		mat4::mat4() {
			for (int i = 0; i < 4 * 4; i++)
				elements[i] = 0.0f;
		}

		mat4::mat4(float diagonal) {
			for(int i = 0; i < 4 * 4; i++)
				elements[i] = 0.0f;

			for (int i = 0; i < 4; i++)
				elements[i + i * 4] = diagonal;
		}

		mat4 mat4::identity() {
			return mat4(1.0f);
		}

		vec4 mat4::getColumn(unsigned int index) {
			return {
				elements[0 + index * 4],
				elements[1 + index * 4],
				elements[2 + index * 4],
				elements[3 + index * 4]
			};
		}

		mat4 mat4::multiply(mat4& other) {
			mat4 rtn;

			vec4 left_one = getColumn(0);
			vec4 left_two = getColumn(1);
			vec4 left_three = getColumn(2);
			vec4 left_four = getColumn(3);

			vec4 right_one = other.getColumn(0);
			vec4 right_two = other.getColumn(1);
			vec4 right_three = other.getColumn(2);
			vec4 right_four = other.getColumn(3);

			vec4 col1 = left_one * right_one[0]   + left_two * right_one[1]   + left_three * right_one[2]   + left_four * right_one[3];
			vec4 col2 = left_one * right_two[0]   + left_two * right_two[1]   + left_three * right_two[2]   + left_four * right_two[3];
			vec4 col3 = left_one * right_three[0] + left_two * right_three[1] + left_three * right_three[2] + left_four * right_three[3];
			vec4 col4 = left_one * right_four[0]  + left_two * right_four[1]  + left_three * right_four[2]  + left_four * right_four[3];

			rtn[0 + 0 * 4] = col1.x;
			rtn[1 + 0 * 4] = col1.y;
			rtn[2 + 0 * 4] = col1.z;
			rtn[3 + 0 * 4] = col1.w;

			rtn[0 + 1 * 4] = col2.x;
			rtn[1 + 1 * 4] = col2.y;
			rtn[2 + 1 * 4] = col2.z;
			rtn[3 + 1 * 4] = col2.w;

			rtn[0 + 2 * 4] = col3.x;
			rtn[1 + 2 * 4] = col3.y;
			rtn[2 + 2 * 4] = col3.z;
			rtn[3 + 2 * 4] = col3.w;

			rtn[0 + 3 * 4] = col4.x;
			rtn[1 + 3 * 4] = col4.y;
			rtn[2 + 3 * 4] = col4.z;
			rtn[3 + 3 * 4] = col4.w;

			return rtn;
		}

		vec4 mat4::multiply(vec4& other) {
			vec4 rtn;

			rtn.x = elements[0 + 0 * 4] + other.x + elements[0 + 1 * 4] + other.y + elements[0 + 2 * 4] + other.z + elements[0 + 3 * 4] + other.w;
			rtn.y = elements[1 + 0 * 4] + other.x + elements[1 + 1 * 4] + other.y + elements[1 + 2 * 4] + other.z + elements[1 + 3 * 4] + other.w;
			rtn.z = elements[2 + 0 * 4] + other.x + elements[2 + 1 * 4] + other.y + elements[2 + 2 * 4] + other.z + elements[2 + 3 * 4] + other.w;
			rtn.w = elements[3 + 0 * 4] + other.x + elements[3 + 1 * 4] + other.y + elements[3 + 2 * 4] + other.z + elements[3 + 3 * 4] + other.w;

			return rtn;
		}

		mat4 mat4::orthographic(float left, float right, float top, float bottom, float near, float far) {
			mat4 result(1.0f);

			result.elements[0 + 0 * 4] = 2.0f / (right - left);
			result.elements[1 + 1 * 4] = 2.0f / (top - bottom);
			result.elements[2 + 2 * 4] = 2.0f / (near - far);

			result.elements[0 + 3 * 4] = (left + right) / (left - right);
			result.elements[1 + 3 * 4] = (bottom + top) / (bottom - top);
			result.elements[2 + 3 * 4] = (far + near) / (far - near);

			return result;
		}

		mat4 mat4::perspective(float fov, float aspectRatio, float near, float far) {
			mat4 result(1.0f);

			float tanfov2 = Trig::tangent(fov / 2);

			result.elements[0 + 0 * 4] = 1 / (aspectRatio * tanfov2);
			result.elements[1 + 1 * 4] = 1 / tanfov2;
			result.elements[2 + 2 * 4] = - ((far + near) / (far - near));
			result.elements[3 + 2 * 4] = -1.0f;
			result.elements[2 + 3 * 4] = - ((2 * far * near) / (far - near));

			return result;
		}

		mat4 mat4::lookAt(vec3 eye, vec3 center, vec3 y) {
			vec3 fwd = vec3::normalize(center - eye);
			vec3 side = vec3::normalize(vec3::cross(fwd, y));
			vec3 up = vec3::cross(side, fwd);

			mat4 rtn;

			rtn[0 + 0 * 4] = side.x;
			rtn[1 + 0 * 4] = up.x;
			rtn[2 + 0 * 4] = -fwd.x;
			rtn[3 + 0 * 4] = 0.f;
			rtn[0 + 1 * 4] = side.y;
			rtn[1 + 1 * 4] = up.y;
			rtn[2 + 1 * 4] = -fwd.y;
			rtn[3 + 1 * 4] = 0.f;
			rtn[0 + 2 * 4] = side.z;
			rtn[1 + 2 * 4] = up.z;
			rtn[2 + 2 * 4] = -fwd.z;
			rtn[3 + 2 * 4] = 0.f;
			rtn[0 + 3 * 4] = -vec3::dot(side, eye);
			rtn[1 + 3 * 4] = -vec3::dot(up, eye);
			rtn[2 + 3 * 4] = vec3::dot(fwd, eye);
			rtn[3 + 3 * 4] = 1.0f;

			return rtn;
		}

		mat4 mat4::translation(vec3 trans) {
			mat4 result(1.0f);

			result.elements[0 + 3 * 4] = trans.x;
			result.elements[1 + 3 * 4] = trans.y;
			result.elements[2 + 3 * 4] = trans.z;

			return result;
		}

		mat4 mat4::rotation(float angle, vec3 axis) {
			mat4 result(1.0f);

			float cosine = Trig::cosine(angle);
			float sine = Trig::sine(angle);
			float neg_cosine = 1.0f - cosine;

			vec3 ax = vec3::normalize(axis);
			float x = ax.x;
			float y = ax.y;
			float z = ax.z;

			result.elements[0 + 0 * 4] = cosine + x * x * neg_cosine;
			result.elements[1 + 0 * 4] = y * x * neg_cosine + z * sine;
			result.elements[2 + 0 * 4] = z * x * neg_cosine - y * sine;
				 
			result.elements[0 + 1 * 4] = x * y * neg_cosine - z * sine;
			result.elements[1 + 1 * 4] = cosine + y * y * neg_cosine;
			result.elements[2 + 1 * 4] = z * y * neg_cosine + x * sine;
			
			result.elements[0 + 2 * 4] = x * z * neg_cosine + y * sine;
			result.elements[1 + 2 * 4] = y * z * neg_cosine - x * sine;
			result.elements[2 + 2 * 4] = cosine + z * z * neg_cosine;

			return result;
		}

		mat4 mat4::scale(vec3 scal) {
			mat4 result(1.0f);

			result.elements[0 + 0 * 4] = scal.x;
			result.elements[1 + 1 * 4] = scal.y;
			result.elements[2 + 2 * 4] = scal.z;

			return result;
		}

		mat4 mat4::inverse(const mat4& m) {
			mat4 inv;

			inv[0] = m[5] * m[10] * m[15] -
					m[5] * m[11] * m[14] -
					m[9] * m[6] * m[15] +
					m[9] * m[7] * m[14] +
					m[13] * m[6] * m[11] -
					m[13] * m[7] * m[10];

			inv[4] = -m[4] * m[10] * m[15] +
					m[4] * m[11] * m[14] +
					m[8] * m[6] * m[15] -
					m[8] * m[7] * m[14] -
					m[12] * m[6] * m[11] +
					m[12] * m[7] * m[10];

			inv[8] = m[4] * m[9] * m[15] -
				m[4] * m[11] * m[13] -
				m[8] * m[5] * m[15] +
				m[8] * m[7] * m[13] +
				m[12] * m[5] * m[11] -
				m[12] * m[7] * m[9];

			inv[12] = -m[4] * m[9] * m[14] +
				m[4] * m[10] * m[13] +
				m[8] * m[5] * m[14] -
				m[8] * m[6] * m[13] -
				m[12] * m[5] * m[10] +
				m[12] * m[6] * m[9];

			inv[1] = -m[1] * m[10] * m[15] +
				m[1] * m[11] * m[14] +
				m[9] * m[2] * m[15] -
				m[9] * m[3] * m[14] -
				m[13] * m[2] * m[11] +
				m[13] * m[3] * m[10];

			inv[5] = m[0] * m[10] * m[15] -
				m[0] * m[11] * m[14] -
				m[8] * m[2] * m[15] +
				m[8] * m[3] * m[14] +
				m[12] * m[2] * m[11] -
				m[12] * m[3] * m[10];

			inv[9] = -m[0] * m[9] * m[15] +
				m[0] * m[11] * m[13] +
				m[8] * m[1] * m[15] -
				m[8] * m[3] * m[13] -
				m[12] * m[1] * m[11] +
				m[12] * m[3] * m[9];

			inv[13] = m[0] * m[9] * m[14] -
				m[0] * m[10] * m[13] -
				m[8] * m[1] * m[14] +
				m[8] * m[2] * m[13] +
				m[12] * m[1] * m[10] -
				m[12] * m[2] * m[9];

			inv[2] = m[1] * m[6] * m[15] -
				m[1] * m[7] * m[14] -
				m[5] * m[2] * m[15] +
				m[5] * m[3] * m[14] +
				m[13] * m[2] * m[7] -
				m[13] * m[3] * m[6];

			inv[6] = -m[0] * m[6] * m[15] +
				m[0] * m[7] * m[14] +
				m[4] * m[2] * m[15] -
				m[4] * m[3] * m[14] -
				m[12] * m[2] * m[7] +
				m[12] * m[3] * m[6];

			inv[10] = m[0] * m[5] * m[15] -
				m[0] * m[7] * m[13] -
				m[4] * m[1] * m[15] +
				m[4] * m[3] * m[13] +
				m[12] * m[1] * m[7] -
				m[12] * m[3] * m[5];

			inv[14] = -m[0] * m[5] * m[14] +
				m[0] * m[6] * m[13] +
				m[4] * m[1] * m[14] -
				m[4] * m[2] * m[13] -
				m[12] * m[1] * m[6] +
				m[12] * m[2] * m[5];

			inv[3] = -m[1] * m[6] * m[11] +
				m[1] * m[7] * m[10] +
				m[5] * m[2] * m[11] -
				m[5] * m[3] * m[10] -
				m[9] * m[2] * m[7] +
				m[9] * m[3] * m[6];

			inv[7] = m[0] * m[6] * m[11] -
				m[0] * m[7] * m[10] -
				m[4] * m[2] * m[11] +
				m[4] * m[3] * m[10] +
				m[8] * m[2] * m[7] -
				m[8] * m[3] * m[6];

			inv[11] = -m[0] * m[5] * m[11] +
				m[0] * m[7] * m[9] +
				m[4] * m[1] * m[11] -
				m[4] * m[3] * m[9] -
				m[8] * m[1] * m[7] +
				m[8] * m[3] * m[5];

			inv[15] = m[0] * m[5] * m[10] -
				m[0] * m[6] * m[9] -
				m[4] * m[1] * m[10] +
				m[4] * m[2] * m[9] +
				m[8] * m[1] * m[6] -
				m[8] * m[2] * m[5];

			float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

			if (det == 0) {
				std::cout << "Mat4 determinant is equal to 0!\n";
				static_assert(true, "Err");
			}

			det = 1.f / det;

			for (unsigned int i = 0; i < 16; i++)
				inv[i] *= det;

			return inv;
		}

		mat4 operator*(mat4 left, const mat4& right) {
			mat4 copy = right;
			return left.multiply(copy);
		}

		mat4 mat4::operator*=(const mat4& other) {
			mat4 copy = other;
			return multiply(copy);
		}

		vec4 operator*(mat4 left, const vec4& right) {
			vec4 copy = right;
			return left.multiply(copy);
		}

		vec4 mat4::operator*=(const vec4& other) {
			vec4 copy = other;
			return multiply(copy);
		}

		float& mat4::operator[](unsigned int index) {
			if (index >= 4 * 4) {
				std::cout << "Index out of bound!\n";
				return elements[0];
			}

			return elements[index];
		}

		const float& mat4::operator[](unsigned int index) const {
			if (index >= 4 * 4) {
				std::cout << "Index out of bound!\n";
				return elements[0];
			}

			return elements[index];
		}

		std::ostream& operator<<(std::ostream& stream, const mat4& matrix) {
			stream << "mat4:"
				<< " | " << matrix[0 + 0 * 4] << " " << matrix[0 + 1 * 4] << " " << matrix[0 + 2 * 4] << " " << matrix[0 + 3 * 4] << " |\n"
				<< "      | " << matrix[1 + 0 * 4] << " " << matrix[1 + 1 * 4] << " " << matrix[1 + 2 * 4] << " " << matrix[1 + 3 * 4] << " |\n"
				<< "      | " << matrix[2 + 0 * 4] << " " << matrix[2 + 1 * 4] << " " << matrix[2 + 2 * 4] << " " << matrix[2 + 3 * 4] << " |\n"
				<< "      | " << matrix[3 + 0 * 4] << " " << matrix[3 + 1 * 4] << " " << matrix[3 + 2 * 4] << " " << matrix[3 + 3 * 4] << " |";

			return stream;
		}
} }