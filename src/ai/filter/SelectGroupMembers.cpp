#include <filter/SelectGroupMembers.h>
#include <zone/Zone.h>

namespace ai {

void SelectGroupMembers::filter (const AI& entity) {
	FilteredEntities& entities = getFilteredEntities(entity);
	auto func = [&] (const ICharacter& chr) {
		entities.push_back(chr.getId());
		return true;
	};
	entity.getZone().getGroupMgr().visit(_groupId, func);
}

FILTER_FACTORY_IMPL(SelectGroupMembers)

}
