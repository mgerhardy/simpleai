#pragma once

#include "common/Vector3f.h"
#include <cmath>

namespace ai {

class MoveVector {
protected:
	const Vector3f _vec3;
	const float _rotation;
public:
	MoveVector(const Vector3f& vec3, float rotation) :
			_vec3(vec3), _rotation(rotation) {
	}

	MoveVector(const Vector3f& vec3, double rotation) :
			_vec3(vec3), _rotation(static_cast<float>(rotation)) {
	}

	inline float getOrientation(float duration) const {
		return fmodf(_rotation * duration, M_2PI);
	}

	inline const Vector3f& getVector() const {
		return _vec3;
	}

	inline Vector3f getVector() {
		return _vec3;
	}

	inline float getRotation() const {
		return _rotation;
	}

	inline operator Vector3f() const {
		return _vec3;
	}

	inline operator const Vector3f&() const {
		return _vec3;
	}

	inline operator float() const {
		return _rotation;
	}
};

inline std::ostream& operator<<(std::ostream& output, const MoveVector& p) {
	return output << "(" << p.getVector() << ", " << p.getRotation() << ", " << p.getOrientation(1.0f) << ")";
}

}
