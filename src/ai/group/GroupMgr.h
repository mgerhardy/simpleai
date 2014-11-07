#pragma once

#include "ICharacter.h"
#include <map>
#include <set>

namespace ai {

typedef int GroupId;
typedef std::set<ICharacter*> GroupMembersSet;
typedef GroupMembersSet::iterator GroupMembersSetIter;
typedef GroupMembersSet::const_iterator GroupMembersSetConstIter;
typedef std::map<GroupId, GroupMembersSet> GroupMembers;
typedef GroupMembers::iterator GroupMembersIter;
typedef GroupMembers::const_iterator GroupMembersConstIter;

/**
 * @brief Maintains the groups a @c ICharacter can be in.
 * @note Keep in mind that if you destroy an @c ICharacter somewhere in the game, to also
 * remove it from the groups
 *
 * TODO: thread safety
 */
class GroupMgr {
private:
	GroupMembersSet _empty;
	GroupMembers _members;

	struct AveragePositionFunctor {
		Vector3f operator()(const Vector3f& result, const ICharacter* chr) {
			return result + chr->getPosition();
		}
	};

public:
	GroupMgr ();
	virtual ~GroupMgr ();

	bool add(GroupId id, ICharacter* character);
	bool remove(GroupId id, ICharacter* character);

	/**
	 * @brief Calculate the average position of the group
	 */
	Vector3f getPosition(GroupId id) const;

	// TODO:
	// wait
	// signal

	/**
	 * @brief Returns a pair of the begin and end iterator of the group members
	 */
	std::pair<GroupMembersSetIter, GroupMembersSetIter> getGroupMembers(GroupId id) const;

	int getGroupSize(GroupId id) const;

	bool isInAnyGroup(const ICharacter& character) const;

	bool isInGroup(GroupId id, const ICharacter& character) const;

	bool isGroupLeader(GroupId id, const ICharacter& character) const;
};

}
