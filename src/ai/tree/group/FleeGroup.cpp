#include "FleeGroup.h"
#include "common/Math.h"
#include "common/Random.h"
#include "movement/Steering.h"

namespace ai {

TreeNodeStatus FleeGroup::doAction(AI& entity, long deltaMillis) {
	const movement::GroupFlee w(_groupId, false);
	if (!w.isValid())
		return FAILED;

	ICharacter& chr = entity.getCharacter();
	const MoveVector& mv = w.execute(chr, chr.getSpeed());
	if (mv.getVector().isInfinite())
		return FAILED;

	const float deltaSeconds = static_cast<float>(deltaMillis) / 1000.0f;
	chr.setPosition(chr.getPosition() + (mv.getVector() * deltaSeconds));
	chr.setOrientation(static_cast<float>(mv.getOrientation(deltaSeconds)));
	return FINISHED;
}

NODE_FACTORY_IMPL(FleeGroup)

}
