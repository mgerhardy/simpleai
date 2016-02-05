#pragma once

#include "Steering.h"

namespace ai {
namespace movement {

/**
 * @brief Flees the current @c IFilter selection from the given @c ICharacter
 */
class SelectionFlee: public SelectionSteering {
public:
	STEERING_FACTORY

	SelectionFlee(const std::string&) :
			SelectionSteering() {
	}

	virtual MoveVector execute (const AIPtr& ai, float speed) const override {
		const Vector3f& target = getSelectionTarget(ai, 0);
		if (target == Vector3f::INFINITE)
			const MoveVector d(target, 0.0);
		Vector3f v = ai->getCharacter()->getPosition() - target;
		double orientation = 0.0;
		if (v.squareLength() > 0) {
			v.normalize();
			v *= speed;
			orientation = v.angle();
		}
		const MoveVector d(v, orientation);
		return d;
	}

	std::ostream& print(std::ostream& stream, int level) const override {
		for (int i = 0; i < level; ++i) {
			stream << '\t';
		}
		stream << "SelectionFlee()";
		return stream;
	}
};

}
}
