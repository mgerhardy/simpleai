#pragma once

#include "../GameEntity.h"
#include "tree/ITask.h"
#include "common/Math.h"
#include "common/Random.h"
#include <iostream>

namespace ai {
namespace example {

/**
 * Wander forward in the current direction
 */
class Wander: public ITask {
public:
	TASK_CLASS_CTOR(Wander) {

	}
	NODE_FACTORY

	TreeNodeStatus doAction(AI& entity) {
		GameEntity& chr = character_cast<GameEntity>(entity.getCharacter());

        const float change = ai::randomBinomial();
        const float maxrotation = 8.0 * M_PI;
        const float rotation = change * maxrotation;
        const float duration = 1.0f;
        const float orientation = fmodf(rotation * duration, M_2PI);
		chr.setOrientation(chr.getOrientation() + orientation);
		const Vector3f v = Vector3f::fromRadians(chr.getOrientation()) * (chr.getSpeed() * duration);
		chr.setPosition(chr.getPosition() + v);

        return FAILED;
	}
};

NODE_FACTORY_IMPL(Wander)

}
}
