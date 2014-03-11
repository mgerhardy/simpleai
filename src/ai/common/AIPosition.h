#pragma once

#include <stdint.h>
#include <cmath>

namespace ai {

class AIPosition {
private:
	float _x;
	float _y;
	float _z;
public:
	AIPosition() :
			_x(0.0f), _y(0.0f), _z(0.0f) {
	}

	AIPosition(const float x, const float y) :
			_x(x), _y(y), _z(0.0f) {
	}

	AIPosition(const AIPosition& pos) :
			_x(pos._x), _y(pos._y), _z(pos._z) {
	}

	AIPosition(const float x, const float y, const float z) :
			_x(x), _y(y), _z(z) {
	}

	virtual ~AIPosition() {
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

	inline void operator +=(const AIPosition& pos) {
		_x += pos._x;
		_y += pos._y;
		_z += pos._z;
	}

	inline void operator -=(const AIPosition& pos) {
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
		return std::sqrt(_x * _x + _y * _y + _z * _z);
	}

	inline bool operator ==(const AIPosition& other) const {
		const float epsilon = 0.00001f;
		return ::fabs(_x - other._x) < epsilon && ::fabs(_y - other._y) < epsilon && ::fabs(_z - other._z) < epsilon;
	}

	inline bool operator !=(const AIPosition& other) const {
		return !(operator ==(other));
	}

	inline float distance(const AIPosition& pos) const {
		const AIPosition c(_x - pos._x, _y - pos._y, _z - pos._z);
		return c.length();
	}

	inline float dot(const AIPosition& pos) const {
		return _x * pos._x + _y * _y + _z * pos._z;
	}

	inline AIPosition cross(const AIPosition& pos) const {
		return AIPosition(_y * pos._z - _z * pos._y, _z * pos._x - _x * pos._z, _x * pos._y - _y * pos._x);
	}
};

}
