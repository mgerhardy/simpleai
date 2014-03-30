#include "GroupMgr.h"

namespace ai {

GroupMgr::GroupMgr() {
}

GroupMgr::~GroupMgr() {
}

bool GroupMgr::add(GroupId id, ICharacter* character) {
	GroupMembersIter i = _members.find(id);
	if (i == _members.end()) {
		GroupMembersSet set;
		i = _members.insert(std::pair<GroupId, GroupMembersSet>(id, set)).first;
	}

	std::pair<GroupMembersSetIter, bool> ret = i->second.insert(character);
	return ret.second;
}

bool GroupMgr::remove(GroupId id, ICharacter* character) {
	GroupMembersIter i = _members.find(id);
	if (i == _members.end())
		return false;
	GroupMembersSetIter si = i->second.find(character);
	if (si == i->second.end())
		return false;
	i->second.erase(si);
	return true;
}

Vector3f GroupMgr::getPosition(GroupId id) const {
	Vector3f averagePosition;
	GroupMembersConstIter i = _members.find(id);
	if (i == _members.end())
		return averagePosition;

	for (GroupMembersSetConstIter si = i->second.begin(); si != i->second.end(); ++si) {
		ICharacter* character = *si;
		averagePosition += character->getPosition();
	}
	averagePosition *= 1.0f / (float) i->second.size();
	return averagePosition;
}

std::pair<GroupMembersSetIter, GroupMembersSetIter> GroupMgr::getGroupMembers(GroupId id) {
	GroupMembersIter i = _members.find(id);
	return std::pair<GroupMembersSetIter, GroupMembersSetIter>(i->second.begin(), i->second.end());
}

}
