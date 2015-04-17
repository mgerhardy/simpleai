#pragma once

#include "Steering.h"

namespace ai {
namespace movement {

/**
 * @brief Flees from a particular group
 */
class GroupFlee: public ISteering {
protected:
	GroupId _groupId;
public:
	STEERING_FACTORY

	GroupFlee(const std::string& parameters) :
			ISteering() {
		_groupId = ::atoi(parameters.c_str());
	}

	inline bool isValid () const {
		return _groupId != -1;
	}

	virtual MoveVector execute (const AIPtr& ai, float speed) const override {
		const Vector3f& target = ai->getGroupPosition(_groupId);
		if (target.isInfinite())
			return MoveVector(target, 0.0f);
		Vector3f v = ai->getCharacter()->getPosition() - target;
		double orientation = 0.0;
		if (v.squareLength() > 0.0f) {
			v.normalize();
			v *= speed;
			orientation = v.angle();
		}
		const MoveVector d(v, orientation);
		return d;
	}

	std::ostream& print(std::ostream& stream, int level) const {
		for (int i = 0; i < level; ++i) {
			stream << '\t';
		}
		stream << "GroupFlee(" << _groupId << ")";
		return stream;
	}
};

}
}
