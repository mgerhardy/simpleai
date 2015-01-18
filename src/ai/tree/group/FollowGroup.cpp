#include "FollowGroup.h"
#include "common/Math.h"
#include "common/Random.h"
#include "movement/Steering.h"

namespace ai {

TreeNodeStatus FollowGroup::doAction(AI& entity, long deltaMillis) {
	if (_groupId == -1)
		return state(entity, FAILED);
	ICharacter& chr = entity.getCharacter();

	const movement::GroupSeek w(entity, chr.getSpeed(), _groupId);
	const MoveVector& mv = w.execute();

	const float deltaSeconds = static_cast<float>(deltaMillis) / 1000.0f;
	chr.setPosition(chr.getPosition() + (mv.getVector() * deltaSeconds));
	chr.setOrientation(static_cast<float>(mv.getVector().orientation()));
	return state(entity, FINISHED);
}

NODE_FACTORY_IMPL(FollowGroup)

}
