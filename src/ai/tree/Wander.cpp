#include "Wander.h"
#include <iostream>

namespace ai {

TreeNodeStatus Wander::doAction(AI& entity, long deltaMillis) {
	ICharacter& chr = entity.getCharacter();
	const MoveVector& mv = _w.execute(chr, chr.getSpeed());
	if (mv.getVector().isInfinite())
		return FAILED;

	const float deltaSeconds = static_cast<float>(deltaMillis) / 1000.0f;
	chr.setPosition(chr.getPosition() + (mv.getVector() * deltaSeconds));
	chr.setOrientation(fmodf(chr.getOrientation() + mv.getRotation() * deltaSeconds, M_2PI));
	return FINISHED;
}

NODE_FACTORY_IMPL(Wander)

}
