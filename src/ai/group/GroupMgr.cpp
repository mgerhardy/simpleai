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
	if (i->second.empty())
		_members.erase(i);
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

std::pair<GroupMembersSetIter, GroupMembersSetIter> GroupMgr::getGroupMembers(GroupId id) const {
	GroupMembersConstIter i = _members.find(id);
	if (i == _members.end()) {
		return std::pair<GroupMembersSetIter, GroupMembersSetIter>(_empty.begin(), _empty.end());
	}
	return std::pair<GroupMembersSetIter, GroupMembersSetIter>(i->second.begin(), i->second.end());
}

bool GroupMgr::isGroupLeader(GroupId id, const ICharacter& character) const {
	const std::pair<GroupMembersSetIter, GroupMembersSetIter>& members = getGroupMembers(id);
	if (members.first == members.second)
		return false;
	if ((*members.first)->getId() == character.getId())
		return true;
	return false;
}

int GroupMgr::getGroupSize(GroupId id) const {
	const std::pair<GroupMembersSetIter, GroupMembersSetIter>& members = getGroupMembers(id);
	return std::distance(members.first, members.second);
}

bool GroupMgr::isInGroup(GroupId id, const ICharacter& character) const {
	const std::pair<GroupMembersSetIter, GroupMembersSetIter>& members = getGroupMembers(id);
	for (GroupMembersSetIter i = members.first; i != members.second; ++i) {
		if ((*i)->getId() == character.getId())
			return true;
	}
	return false;
}

}
