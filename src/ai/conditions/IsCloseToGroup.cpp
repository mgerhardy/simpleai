#include "IsCloseToGroup.h"
#include "ICharacter.h"
#include "AI.h"
#include "server/Zone.h"

namespace ai {

bool IsCloseToGroup::evaluate(const AI& entity) {
	if (_groupId == -1)
		return false;

	if (_distance < 0.0f)
		return false;

	GroupMgr& mgr = entity.getZone().getGroupMgr();
	const Vector3f& pos = mgr.getPosition(_groupId);
	if (pos.isInfinite())
		return false;
	return pos.distance(entity.getCharacter().getPosition()) <= _distance;
}

CONDITION_FACTORY_IMPL(IsCloseToGroup)

}
