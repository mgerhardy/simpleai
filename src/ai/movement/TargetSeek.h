#pragma once

#include "Steering.h"

namespace ai {
namespace movement {

/**
 * @brief Seeks a particular target
 */
class TargetSeek: public ISteering {
protected:
	Vector3f _target;
public:
	STEERING_FACTORY

	TargetSeek(const std::string& parameters) :
			ISteering() {
		_target = Vector3f::parse(parameters);
	}

	inline bool isValid () const {
		return !_target.isInfinite();
	}

	virtual MoveVector execute (const ICharacter& character, float speed) const override {
		if (_target.isInfinite())
			return MoveVector(_target, 0.0f);
		Vector3f v = _target - character.getPosition();
		double orientation = 0.0;
		if (v.squareLength() > 0.0f) {
			v.normalize();
			v *= speed;
			orientation = v.orientation();
		}
		const MoveVector d(v, orientation);
		return d;
	}

	std::ostream& print(std::ostream& stream, int level) const {
		for (int i = 0; i < level; ++i) {
			stream << '\t';
		}
		stream << "TargetSeek(" << _target << ")";
		return stream;
	}
};

}
}
