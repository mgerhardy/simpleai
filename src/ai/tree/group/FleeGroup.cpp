#include "FleeGroup.h"
#include "common/Math.h"
#include "common/Random.h"
#include "movement/Steering.h"

namespace ai {

TreeNodeStatus FleeGroup::doAction(AI& entity, long deltaMillis) {
	if (_groupId == -1)
		return FAILED;
	ICharacter& chr = entity.getCharacter();

	const movement::GroupFlee w(entity, chr.getSpeed(), _groupId);
	if (!w.isValid())
		return FAILED;
	const MoveVector& mv = w.execute();

	const float deltaSeconds = static_cast<float>(deltaMillis) / 1000.0f;
	chr.setPosition(chr.getPosition() + (mv.getVector() * deltaSeconds));
	chr.setOrientation(static_cast<float>(mv.getOrientation(deltaSeconds)));
	return FINISHED;
}

NODE_FACTORY_IMPL(FleeGroup)

}
