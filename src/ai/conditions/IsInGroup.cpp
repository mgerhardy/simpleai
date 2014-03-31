#include "IsInGroup.h"
#include "AI.h"

namespace ai {

bool IsInGroup::evaluate(AI& entity) override {
	return entity.getGroupMgr().isInGroup(_groupId, entity.getCharacter());
}

CONDITION_FACTORY_IMPL(IsInGroup)

}
