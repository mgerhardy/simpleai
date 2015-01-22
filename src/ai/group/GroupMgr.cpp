#include "GroupMgr.h"
#include "ICharacter.h"
#include <numeric>

namespace ai {

struct AveragePositionFunctor {
	Vector3f operator()(const Vector3f& result, const ICharacter* chr) {
		return chr->getPosition() + result;
	}
};

GroupMgr::GroupMgr() {
}

GroupMgr::~GroupMgr() {
}

bool GroupMgr::add(GroupId id, ICharacter* character) {
	ScopedWriteLock scopedLock(_lock);
	GroupsIter i = _groups.find(id);
	if (i == _groups.end()) {
		Group group;
		group.leader = character;
		i = _groups.insert(std::pair<GroupId, Group>(id, group)).first;
	}

	std::pair<GroupMembersSetIter, bool> ret = i->second.members.insert(character);
	if (ret.second) {
		_groupMembers.insert(GroupMembers::value_type(character, id));
		return true;
	}
	return false;
}

bool GroupMgr::remove(GroupId id, ICharacter* character) {
	ScopedWriteLock scopedLock(_lock);
	const GroupsIter& i = _groups.find(id);
	if (i == _groups.end())
		return false;
	const GroupMembersSetIter& si = i->second.members.find(character);
	if (si == i->second.members.end())
		return false;
	i->second.members.erase(si);

	if (i->second.members.empty())
		_groups.erase(i);
	else if (i->second.leader == character)
		i->second.leader = *i->second.members.begin();
	auto range = _groupMembers.equal_range(character);
	for (auto it = range.first; it != range.second; ++it) {
		if (it->second == id) {
			_groupMembers.erase(it);
			break;
		}
	}
	return true;
}

Vector3f GroupMgr::getPosition(GroupId id) const {
	ScopedReadLock scopedLock(_lock);
	const GroupsConstIter& i = _groups.find(id);
	if (i == _groups.end())
		return Vector3f::INFINITE;

	// TODO: only those that are in the same zone - otherwise the avg pos doesn't make much sense
	Vector3f averagePosition = std::accumulate(i->second.members.begin(), i->second.members.end(), Vector3f(), AveragePositionFunctor());
	averagePosition *= 1.0f / (float) i->second.members.size();
	return averagePosition;
}

bool GroupMgr::isGroupLeader(GroupId id, const ICharacter& character) const {
	ScopedReadLock scopedLock(_lock);
	const GroupsConstIter& i = _groups.find(id);
	if (i == _groups.end()) {
		return 0;
	}
	return i->second.leader == &character;
}

int GroupMgr::getGroupSize(GroupId id) const {
	ScopedReadLock scopedLock(_lock);
	const GroupsConstIter& i = _groups.find(id);
	if (i == _groups.end()) {
		return 0;
	}
	return static_cast<int>(std::distance(i->second.members.begin(), i->second.members.end()));
}

bool GroupMgr::isInAnyGroup(const ICharacter& character) const {
	ScopedReadLock scopedLock(_lock);
	return _groupMembers.find(&character) != _groupMembers.end();
}

bool GroupMgr::isInGroup(GroupId id, const ICharacter& character) const {
	ScopedReadLock scopedLock(_lock);
	auto range = _groupMembers.equal_range(&character);
	for (auto it = range.first; it != range.second; ++it) {
		if (it->second == id)
			return true;
	}
	return false;
}

}
