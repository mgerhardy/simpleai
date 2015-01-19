#include "IsCloseToGroup.h"
#include "ICharacter.h"
#include "AI.h"

namespace ai {

bool IsCloseToGroup::evaluate(const AI& entity) {
	if (_groupId == -1)
		return false;

	if (_distance < 0.0f)
		return false;

	const Vector3f& pos = entity.getGroupMgr().getPosition(_groupId);
	if (pos.isInfinite())
		return false;
	return pos.distance(entity.getCharacter().getPosition()) <= _distance;
}

CONDITION_FACTORY_IMPL(IsCloseToGroup)

}
