#include "Wander.h"
#include "common/Random.h"
#include "movement/Steering.h"
#include <iostream>

namespace ai {

TreeNodeStatus Wander::doAction(AI& entity, long deltaMillis) {
	ICharacter& chr = entity.getCharacter();

	const movement::Wander w(_rotation);
	const MoveVector& mv = w.execute(chr, chr.getSpeed());

	const float deltaSeconds = static_cast<float>(deltaMillis) / 1000.0f;
	chr.setPosition(chr.getPosition() + (mv.getVector() * deltaSeconds));
	chr.setOrientation(fmodf(chr.getOrientation() + mv.getRotation() * deltaSeconds, M_2PI));
	return FINISHED;
}

NODE_FACTORY_IMPL(Wander)

}
