#include "IsInGroup.h"
#include "AI.h"

namespace ai {

bool IsInGroup::evaluate(AI& entity) override {
	if (_groupId == -1)
		return entity.getGroupMgr().isInAnyGroup(entity.getCharacter());
	return entity.getGroupMgr().isInGroup(_groupId, entity.getCharacter());
}

CONDITION_FACTORY_IMPL(IsInGroup)

}
