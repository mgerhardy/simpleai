#pragma once

#include "common/Thread.h"
#include "common/Types.h"
#include "common/Vector3f.h"
#include <map>
#include <unordered_map>
#include <set>

namespace ai {

class ICharacter;

typedef int GroupId;
typedef std::set<ICharacter*> GroupMembersSet;
typedef GroupMembersSet::iterator GroupMembersSetIter;
typedef GroupMembersSet::const_iterator GroupMembersSetConstIter;
typedef std::unordered_map<GroupId, GroupMembersSet> GroupMembers;
typedef GroupMembers::iterator GroupMembersIter;
typedef GroupMembers::const_iterator GroupMembersConstIter;

/**
 * @brief Maintains the groups a @c ICharacter can be in.
 * @note Keep in mind that if you destroy an @c ICharacter somewhere in the game, to also
 * remove it from the groups
 */
class GroupMgr {
private:
	GroupMembersSet _empty;
	GroupMembers _members;
	ReadWriteLock _lock;

public:
	GroupMgr ();
	virtual ~GroupMgr ();

	/**
	 * @brief Adds a new group member to the given @c GroupId.
	 *
	 * @param character The @c ICharacter to add to the group. Keep
	 * in mind that you have to remove it manually from any group
	 * whenever you destroy the @c ICharacter instance.
	 * @return @c true if the add to the group was successful.
	 */
	bool add(GroupId id, ICharacter* character);

	/**
	 * @brief Removes a group member from the given @c GroupId.
	 *
	 * @param character The @c ICharacter to remove from this the group.
	 * @return @c true if the given character was removed from the group,
	 * @c false if the removal failed (e.g. the character was not part of
	 * the group)
	 */
	bool remove(GroupId id, ICharacter* character);

	/**
	 * @brief Calculate the average position of the group
	 */
	Vector3f getPosition(GroupId id) const;

	/**
	 * @brief Visit all the group members of the given group until the functor returns @c false
	 */
	template<typename Func>
	void visit(GroupId id, Func& func) const {
		ScopedReadLock scopedLock(_lock);
		const GroupMembersConstIter& i = _members.find(id);
		if (i == _members.end()) {
			return;
		}
		for (GroupMembersSetConstIter it = i->second.begin(); it != i->second.end(); ++it) {
			const ICharacter* chr = *it;
			if (!func(*chr))
				break;
		}
	}

	int getGroupSize(GroupId id) const;

	bool isInAnyGroup(const ICharacter& character) const;

	bool isInGroup(GroupId id, const ICharacter& character) const;

	bool isGroupLeader(GroupId id, const ICharacter& character) const;
};

}
