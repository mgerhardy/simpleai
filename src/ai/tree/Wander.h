#pragma once

#include "tree/ITask.h"
#include "common/Math.h"
#include "common/String.h"
#include "common/Random.h"
#include "movement/Steering.h"
#include <iostream>

namespace ai {

/**
 * Wander forward in the current direction
 */
class Wander: public ITask {
protected:
	float _rotation;
public:
	TASK_CLASS_CTOR(Wander) {
		if (_parameters.empty()) {
			_rotation = 8.0 * M_PI;
		} else {
			_rotation = Str::toFloat(_parameters);
		}
	}
	NODE_FACTORY

	TreeNodeStatus doAction(AI& entity, long deltaMillis) override {
		ICharacter& chr = entity.getCharacter();
		const float deltaSeconds = static_cast<float>(deltaMillis) / 1000.0f;

		const ai::movement::Wander w(chr, chr.getSpeed(), _rotation);
		const ai::movement::SteeringData& d = w.execute();

		chr.setPosition(chr.getPosition() + (d.linear * deltaSeconds));
		chr.setOrientation(fmodf(chr.getOrientation() + d.angular * deltaSeconds, M_2PI));
		return FINISHED;
	}
};

NODE_FACTORY_IMPL(Wander)

}
