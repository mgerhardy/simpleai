#include <filter/SelectGroupMembers.h>

namespace ai {

void SelectGroupMembers::filter (const AI& entity) {
	FilteredEntities& entities = getFilteredEntities(entity);
	const std::pair<GroupMembersSetIter, GroupMembersSetIter>& members = entity.getGroupMgr().getGroupMembers(_groupId);
	for (GroupMembersSetIter i = members.first; i != members.second; ++i) {
		entities.push_back((*i)->getId());
	}
}

FILTER_FACTORY_IMPL(SelectGroupMembers)

}
