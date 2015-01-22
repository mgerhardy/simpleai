#include "IsInGroup.h"
#include "AI.h"
#include "server/Zone.h"

namespace ai {

bool IsInGroup::evaluate(const AI& entity) {
	GroupMgr& mgr = entity.getZone().getGroupMgr();
	if (_groupId == -1)
		return mgr.isInAnyGroup(entity.getCharacter());
	return mgr.isInGroup(_groupId, entity.getCharacter());
}

CONDITION_FACTORY_IMPL(IsInGroup)

}
