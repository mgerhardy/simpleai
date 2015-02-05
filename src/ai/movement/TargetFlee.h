#pragma once

#include "Steering.h"

namespace ai {
namespace movement {

/**
 * @brief Flees from a particular target
 */
class TargetFlee: public ISteering {
protected:
	Vector3f _target;
public:
	STEERING_FACTORY

	TargetFlee(const std::string& parameters) :
			ISteering() {
		_target = Vector3f::parse(parameters);
	}

	inline bool isValid () const {
		return !_target.isInfinite();
	}

	virtual MoveVector execute (const ICharacter& character, float speed) const override {
		if (_target.isInfinite())
			return MoveVector(_target, 0.0f);
		Vector3f v = character.getPosition();
		v -= _target;
		float orientation = 0.0f;
		if (v.squareLength() > 0) {
			v.normalize();
			v *= speed;
			orientation = v.orientation();
		}

		const MoveVector d(v, 0.0f);
		return d;
	}
};


}
}
