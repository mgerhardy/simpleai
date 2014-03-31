#pragma once

#include <stdint.h>
#include <cmath>

namespace ai {

class Vector3f {
private:
	float _x;
	float _y;
	float _z;
public:
	Vector3f() :
			_x(0.0f), _y(0.0f), _z(0.0f) {
	}

	Vector3f(const float x, const float y) :
			_x(x), _y(y), _z(0.0f) {
	}

	Vector3f(const Vector3f& pos) :
			_x(pos._x), _y(pos._y), _z(pos._z) {
	}

	Vector3f(const float x, const float y, const float z) :
			_x(x), _y(y), _z(z) {
	}

	virtual ~Vector3f() {
	}

	inline void setX(float x) {
		_x = x;
	}

	inline void setY(float y) {
		_y = y;
	}

	inline void setZ(float z) {
		_z = z;
	}

	inline float x() const {
		return _x;
	}

	inline float y() const {
		return _y;
	}

	inline float z() const {
		return _z;
	}

	inline void operator +=(const Vector3f& pos) {
		_x += pos._x;
		_y += pos._y;
		_z += pos._z;
	}

	inline Vector3f operator +(const Vector3f& pos) const {
		return Vector3f(_x + pos._x, _y + pos._y, _z + pos._z);
	}

	inline Vector3f operator *(const float scalar) const {
		return Vector3f(_x * scalar, _y * scalar, _z * scalar);
	}

	inline void operator -=(const Vector3f& pos) {
		_x -= pos._x;
		_y -= pos._y;
		_z -= pos._z;
	}

	inline void operator *=(const float scalar) {
		_x *= scalar;
		_y *= scalar;
		_z *= scalar;
	}

	inline float length() const {
		return std::sqrt(squareLength());
	}

	inline float squareLength() const {
		return _x * _x + _y * _y + _z * _z;
	}

	inline double angle() const {
		const double angle = ::atan2(_y, _x);
		return angle;
	}

	inline double radians() const {
		const double radians = M_PI / 180.0 * angle();
		return radians;
	}

	static inline Vector3f fromRadians(double radians) {
        return Vector3f(sin(radians), 0.0, cos(radians));
	}

	inline Vector3f mix(const Vector3f& end, const float mix) {
		const float number = 1.0f - mix;
		const float x = _x * number + end._x * mix;
		const float y = _y * number + end._y * mix;
		const float z = _z * number + end._z * mix;
		return Vector3f(x, y, z);
	}

	inline Vector3f normalize() const {
		const float norm = 1.0f / length();
		const float x = _x * norm;
		const float y = _y * norm;
		const float z = _z * norm;
		return Vector3f(x, y, z);
	}

	inline Vector3f advance(const Vector3f& direction, const float scale) const {
		const float x = _x + scale * direction._x;
		const float y = _y + scale * direction._y;
		const float z = _z + scale * direction._z;
		return Vector3f(x, y, z);
	}

	inline Vector3f inverse() const {
		const float x = -_x;
		const float y = -_y;
		const float z = -_z;
		return Vector3f(x, y, z);
	}

	inline void inverse() {
		_x = -_x;
		_y = -_y;
		_z = -_z;
	}

	inline float normalize() {
		const float l = length();
		const float norm = 1.0f / l;
		_x *= norm;
		_y *= norm;
		_z *= norm;
		return l;
	}

	inline bool operator ==(const Vector3f& other) const {
		const float epsilon = 0.00001f;
		return ::fabs(_x - other._x) < epsilon && ::fabs(_y - other._y) < epsilon && ::fabs(_z - other._z) < epsilon;
	}

	inline bool operator !=(const Vector3f& other) const {
		return !(operator ==(other));
	}

	inline float distance(const Vector3f& pos) const {
		const Vector3f c(_x - pos._x, _y - pos._y, _z - pos._z);
		return c.length();
	}

	inline float dot(const Vector3f& pos) const {
		return _x * pos._x + _y * _y + _z * pos._z;
	}

	inline Vector3f cross(const Vector3f& pos) const {
		return Vector3f(_y * pos._z - _z * pos._y, _z * pos._x - _x * pos._z, _x * pos._y - _y * pos._x);
	}
};

}
