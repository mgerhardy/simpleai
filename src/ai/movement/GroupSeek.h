#pragma once

#include "Steering.h"

namespace ai {
namespace movement {

/**
 * @brief Seeks a particular group
 */
class GroupSeek: public ISteering {
protected:
	GroupId _groupId;
public:
	STEERING_FACTORY

	GroupSeek(const std::string& parameters) :
			ISteering() {
		_groupId = ::atoi(parameters.c_str());
	}

	inline bool isValid () const {
		return _groupId != -1;
	}

	virtual MoveVector execute (const ICharacter& character, float speed) const override {
		const AI& ai = character.getAI();
		const Vector3f& target = ai.getGroupPosition(_groupId);
		if (target.isInfinite())
			return MoveVector(target, 0.0f);
		Vector3f v = target - character.getPosition();
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
