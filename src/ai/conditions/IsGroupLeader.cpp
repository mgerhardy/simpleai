#include "IsGroupLeader.h"
#include "AI.h"
#include "server/Zone.h"

namespace ai {

bool IsGroupLeader::evaluate(const AI& entity) {
	if (_groupId == -1)
		return false;
	GroupMgr& mgr = entity.getZone().getGroupMgr();
	return mgr.isGroupLeader(_groupId, entity.getCharacter());
}

CONDITION_FACTORY_IMPL(IsGroupLeader)

}
