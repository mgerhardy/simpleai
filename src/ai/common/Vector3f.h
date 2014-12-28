#pragma once

#include <stdint.h>
#include <cmath>
#include <iostream>

namespace ai {

struct Vector3f {
	float x;
	float y;
	float z;

	Vector3f() noexcept:
			x(0.0f), y(0.0f), z(0.0f) {
	}

	Vector3f(const float _x, const float _y) noexcept:
			x(_x), y(_y), z(0.0f) {
	}

	Vector3f(const Vector3f& pos) noexcept:
			x(pos.x), y(pos.y), z(pos.z) {
	}

	Vector3f(const float _x, const float _y, const float _z) :
			x(_x), y(_y), z(_z) {
	}

	inline void operator +=(const Vector3f& pos) {
		x += pos.x;
		y += pos.y;
		z += pos.z;
	}

	inline Vector3f operator -() const {
		return inverse();
	}

	inline float operator () (int32_t i) const {
		return (&x)[i];
	}

	inline float& operator () (int32_t i) {
		return (&x)[i];
	}

	inline Vector3f operator *(const float scalar) const {
		return Vector3f(x * scalar, y * scalar, z * scalar);
	}

	inline void operator -=(const Vector3f& pos) {
		x -= pos.x;
		y -= pos.y;
		z -= pos.z;
	}

	inline void operator *=(const float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
	}

	inline float length() const {
		return std::sqrt(squareLength());
	}

	inline float squareLength() const {
		return x * x + y * y + z * z;
	}

	inline double angle() const {
		const double _angle = ::atan2(z, x);
		return _angle;
	}

	inline double orientation() const {
		const double _orientation = ::atan2(x, z);
		return _orientation;
	}

	inline double radians() const {
		const double _radians = M_PI / 180.0 * angle();
		return _radians;
	}

	static inline Vector3f fromRadians(float radians) {
		return Vector3f(sinf(radians), 0.0f, cosf(radians));
	}

	inline Vector3f mix(const Vector3f& end, const float _mix) {
		const float number = 1.0f - _mix;
		const float _x = x * number + end.x * _mix;
		const float _y = y * number + end.y * _mix;
		const float _z = z * number + end.z * _mix;
		return Vector3f(_x, _y, _z);
	}

	inline Vector3f normalize() const {
		const float norm = 1.0f / length();
		const float _x = x * norm;
		const float _y = y * norm;
		const float _z = z * norm;
		return Vector3f(_x, _y, _z);
	}

	inline Vector3f advance(const Vector3f& direction, const float scale) const {
		const float _x = x + scale * direction.x;
		const float _y = y + scale * direction.y;
		const float _z = z + scale * direction.z;
		return Vector3f(_x, _y, _z);
	}

	inline Vector3f inverse() const {
		const float _x = -x;
		const float _y = -y;
		const float _z = -z;
		return Vector3f(_x, _y, _z);
	}

	inline void inverse() {
		x = -x;
		y = -y;
		z = -z;
	}

	inline float normalize() {
		const float l = length();
		const float norm = 1.0f / l;
		x *= norm;
		y *= norm;
		z *= norm;
		return l;
	}

	inline bool operator ==(const Vector3f& other) const {
		const float epsilon = 0.00001f;
		return ::fabs(x - other.x) < epsilon && ::fabs(y - other.y) < epsilon && ::fabs(z - other.z) < epsilon;
	}

	inline bool operator !=(const Vector3f& other) const {
		return !(operator ==(other));
	}

	inline float distance(const Vector3f& pos) const {
		const Vector3f c(x - pos.x, y - pos.y, z - pos.z);
		return c.length();
	}

	inline float dot(const Vector3f& pos) const {
		return x * pos.x + y * y + z * pos.z;
	}

	inline Vector3f cross(const Vector3f& pos) const {
		return Vector3f(y * pos.z - z * pos.y, z * pos.x - x * pos.z, x * pos.y - y * pos.x);
	}

	static Vector3f ZERO;
};

inline std::ostream& operator<<(std::ostream& output, const Vector3f& p) {
	return output << "(" << p.x << "," << p.y << "," << p.z << ")";
}

inline Vector3f operator-(const Vector3f& lhs, const Vector3f& rhs) {
	const float x = lhs.x - rhs.x;
	const float y = lhs.y - rhs.y;
	const float z = lhs.z - rhs.z;
	return Vector3f(x, y, z);
}

inline Vector3f operator+(const Vector3f& lhs, const Vector3f& rhs) {
	const float x = lhs.x + rhs.x;
	const float y = lhs.y + rhs.y;
	const float z = lhs.z + rhs.z;
	return Vector3f(x, y, z);
}

inline Vector3f operator*(float scalar, const Vector3f& a) {
	return Vector3f(scalar * a.x, scalar * a.y, scalar * a.z);
}

}
