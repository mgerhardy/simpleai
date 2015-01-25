#pragma once

#include "common/Thread.h"
#include "common/Types.h"
#include "common/Vector3f.h"
#include <map>
#include <unordered_map>
#include <unordered_set>

namespace ai {

class ICharacter;

typedef int GroupId;

/**
 * @brief Maintains the groups a @c ICharacter can be in.
 * @note Keep in mind that if you destroy an @c ICharacter somewhere in the game, to also
 * remove it from the groups
 */
class GroupMgr {
private:
	typedef std::unordered_set<ICharacter*> GroupMembersSet;
	typedef GroupMembersSet::iterator GroupMembersSetIter;
	typedef GroupMembersSet::const_iterator GroupMembersSetConstIter;

	struct Group {
		ICharacter* leader;
		GroupMembersSet members;
		Vector3f position;
	};

	typedef std::unordered_multimap<const ICharacter*, GroupId> GroupMembers;
	typedef std::unordered_map<GroupId, Group> Groups;
	typedef Groups::const_iterator GroupsConstIter;
	typedef Groups::iterator GroupsIter;

	GroupMembersSet _empty;
	ReadWriteLock _lock;
	Groups _groups;
	GroupMembers _groupMembers;

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

	void update(long deltaTime);

	/**
	 * @brief Removes a group member from the given @c GroupId.
	 *
	 * @param character The @c ICharacter to remove from this the group.
	 * @return @c true if the given character was removed from the group,
	 * @c false if the removal failed (e.g. the character was not part of
	 * the group)
	 */
	bool remove(GroupId id, ICharacter* character);

	bool removeFromAllGroups(ICharacter* character);

	/**
	 * @brief Calculate the average position of the group
	 *
	 * @note If the given group doesn't exist or some other error occurred, this method returns @c Vector3f::INFINITE
	 */
	Vector3f getPosition(GroupId id) const;

	/**
	 * @return The @c ICharacter object of the leader, or @c nullptr if no such group exists.
	 */
	const ICharacter* getLeader(GroupId id) const;

	/**
	 * @brief Visit all the group members of the given group until the functor returns @c false
	 */
	template<typename Func>
	void visit(GroupId id, Func& func) const {
		ScopedReadLock scopedLock(_lock);
		const GroupsConstIter& i = _groups.find(id);
		if (i == _groups.end()) {
			return;
		}
		for (GroupMembersSetConstIter it = i->second.members.begin(); it != i->second.members.end(); ++it) {
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
