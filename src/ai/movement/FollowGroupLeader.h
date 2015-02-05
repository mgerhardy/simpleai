#pragma once

#include "Steering.h"

namespace ai {
namespace movement {

/**
 * @brief Flees from a particular group
 */
class FollowGroupLeader: public ISteering {
protected:
	GroupId _groupId;
public:
	STEERING_FACTORY

	FollowGroupLeader(const std::string& parameters) :
			ISteering() {
		_groupId = ::atoi(parameters.c_str());
	}

	inline bool isValid () const {
		return _groupId != -1;
	}

	virtual MoveVector execute (const ICharacter& character, float speed) const override {
		const AI& ai = character.getAI();
		const Vector3f& target = ai.getGroupLeaderPosition(_groupId);
		if (target.isInfinite())
			return MoveVector(target, 0.0f);
		Vector3f v = character.getPosition() - target;
		double orientation = 0.0;
		if (v.squareLength() > 0) {
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
		stream << "FollowGroupLeader(" << _groupId << ")";
		return stream;
	}
};

}
}
