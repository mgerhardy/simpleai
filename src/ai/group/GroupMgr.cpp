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
	GroupMembersIter i = _members.find(id);
	if (i == _members.end()) {
		GroupMembersSet set;
		i = _members.insert(std::pair<GroupId, GroupMembersSet>(id, set)).first;
	}

	std::pair<GroupMembersSetIter, bool> ret = i->second.insert(character);
	return ret.second;
}

bool GroupMgr::remove(GroupId id, ICharacter* character) {
	ScopedWriteLock scopedLock(_lock);
	const GroupMembersIter& i = _members.find(id);
	if (i == _members.end())
		return false;
	const GroupMembersSetIter& si = i->second.find(character);
	if (si == i->second.end())
		return false;
	i->second.erase(si);
	if (i->second.empty())
		_members.erase(i);
	return true;
}

Vector3f GroupMgr::getPosition(GroupId id) const {
	ScopedReadLock scopedLock(_lock);
	const GroupMembersConstIter& i = _members.find(id);
	if (i == _members.end())
		return Vector3f::ZERO;

	// TODO: only those that are in the same zone - otherwise the avg pos doesn't make much sense
	Vector3f averagePosition = std::accumulate(i->second.begin(), i->second.end(), Vector3f(), AveragePositionFunctor());
	averagePosition *= 1.0f / (float) i->second.size();
	return averagePosition;
}

bool GroupMgr::isGroupLeader(GroupId id, const ICharacter& character) const {
	bool leader = false;
	auto func = [&] (const ICharacter& chr) {
		leader = chr.getId() == character.getId();
		return false;
	};
	visit(id, func);
	return leader;
}

int GroupMgr::getGroupSize(GroupId id) const {
	ScopedReadLock scopedLock(_lock);
	const GroupMembersConstIter& i = _members.find(id);
	if (i == _members.end()) {
		return 0;
	}
	return static_cast<int>(std::distance(i->second.begin(), i->second.end()));
}

bool GroupMgr::isInAnyGroup(const ICharacter& character) const {
	ScopedReadLock scopedLock(_lock);
	for (GroupMembersConstIter i = _members.begin(); i != _members.end(); ++i) {
		const GroupMembersSetConstIter& it = i->second.find(const_cast<ICharacter*>(&character));
		if (it != i->second.end())
			return true;
	}
	return false;
}

bool GroupMgr::isInGroup(GroupId id, const ICharacter& character) const {
	bool inGroup = false;
	auto func = [&] (const ICharacter& chr) {
		if (chr.getId() == character.getId())
			inGroup = true;
		return !inGroup;
	};
	visit(id, func);
	return inGroup;
}

}
