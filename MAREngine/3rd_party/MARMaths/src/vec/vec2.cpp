/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "vec2.h"

namespace mar { 
	namespace maths {

	vec2::vec2() {
		this->x = 0.0f;
		this->y = 0.0f;
	}

	vec2::vec2(const float& x, const float& y) {
		this->x = x;
		this->y = y;
	}

	vec2::vec2(const float&& x, const float&& y) {
		this->x = x;
		this->y = y;
	}

	vec2& vec2::add(const float& f) {
		x += f;
		y += f;

		return *this;
	}

	vec2& vec2::subtract(const float& f) {
		x -= f;
		y -= f;

		return *this;
	}

	vec2& vec2::multiply(const float& f) {
		x *= f;
		y *= f;

		return *this;
	}

	vec2& vec2::divide(const float& f) {
		if (f == 0.f) return *this;

		x /= f;
		y /= f;

		return *this;
	}

	vec2& vec2::add(const vec2& other) {
		x += other.x;
		y += other.y;

		return *this;
	}

	vec2& vec2::subtract(const vec2& other) {
		x -= other.x;
		y -= other.y;

		return *this;
	}

	vec2& vec2::multiply(const vec2& other) {
		x *= other.x;
		y *= other.y;

		return *this;
	}

	vec2& vec2::divide(const vec2& other) {
		if (other.x == 0 || other.y == 0) {
			return *this;
		}

		x /= other.x;
		y /= other.y;

		return *this;
	}

	float vec2::dot(const vec2& other) {
		return dot(*this, other);
	}

	float vec2::dot(const vec2& left, const vec2& right) {
		return left.x * right.x + left.y * right.y;
	}

	float vec2::length() const {
		return basic::square(dot(*this, *this));
	}

	float vec2::length(vec2& v) {
		return v.length();
	}

	float vec2::length(vec2&& v) {
		return v.length();
	}

	vec2 vec2::normalize(const vec2& other) {
		float magnitude = basic::square((other.x * other.x) + (other.y * other.y));

		vec2 rtn{ other.x / magnitude, other.y / magnitude };

		return rtn;
	}

	vec2 operator+(vec2 left, float right) {
		return left.add(right);
	}

	vec2 operator-(vec2 left, float right) {
		return left.subtract(right);
	}

	vec2 operator*(vec2 left, float right) {
		return left.multiply(right);
	}

	vec2 operator/(vec2 left, float right) {
		return left.divide(right);
	}

	vec2 operator+(vec2 left, const vec2& right) {
		return left.add(right);
	}

	vec2 operator-(vec2 left, const vec2& right) {
		return left.subtract(right);
	}

	vec2 operator*(vec2 left, const vec2& right) {
		return left.multiply(right);
	}

	vec2 operator/(vec2 left, const vec2& right) {
		return left.divide(right);
	}

	vec2& vec2::operator+=(const float& other) {
		return add(other);
	}

	vec2& vec2::operator-=(const float& other) {
		return subtract(other);
	}

	vec2& vec2::operator*=(const float& other) {
		return multiply(other);
	}

	vec2& vec2::operator/=(const float& other) {
		return divide(other);
	}

	vec2& vec2::operator+=(const vec2& other) {
		return add(other);
	}

	vec2& vec2::operator-=(const vec2& other) {
		return subtract(other);
	}

	vec2& vec2::operator*=(const vec2& other) {
		return multiply(other);
	}

	vec2& vec2::operator/=(const vec2& other) {
		return divide(other);
	}

	bool vec2::operator==(const vec2& other) {
		return x == other.x && y == other.y;
	}

	bool vec2::operator!=(const vec2& other) {
		return !(*this == other);
	}

	std::ostream& operator<<(std::ostream& stream, const vec2& vector) {
		stream << "vec2: ( " << vector.x << " , " << vector.y << " )";

		return stream;
	}

} }
