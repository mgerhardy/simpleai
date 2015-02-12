#include <filter/SelectGroupLeader.h>
#include <zone/Zone.h>

namespace ai {

void SelectGroupLeader::filter (const AI& entity) {
	FilteredEntities& entities = getFilteredEntities(entity);
	const ICharacter* groupLeader = entity.getZone().getGroupMgr().getLeader(_groupId);
	if (groupLeader != nullptr) {
		entities.push_back(groupLeader->getId());
	}
}

FILTER_FACTORY_IMPL(SelectGroupLeader)

}
