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
		if (v.squareLength() > 0) {
			v.normalize();
			v *= speed;
		}
		const MoveVector d(v, 0.0f);
		return d;
	}
};

}
}
