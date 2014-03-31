#include "IsCloseToGroup.h"
#include "AI.h"

namespace ai {

bool IsCloseToGroup::evaluate(AI& entity) override {
	if (_groupId == -1)
		return false;

	if (_distance < 0.0f)
		return false;

	return entity.getGroupMgr().getPosition(_groupId).distance(entity.getCharacter().getPosition()) <= _distance;
}

CONDITION_FACTORY_IMPL(IsCloseToGroup)

}
