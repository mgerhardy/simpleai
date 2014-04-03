#pragma once

#include <map>

#include "aggro/AggroMgr.h"
#include "tree/loaders/ITreeLoader.h"
#include "ICharacter.h"
#include "pathfinding/IPathfinder.h"
#include "group/GroupMgr.h"

namespace ai {

#define NOTHING_SELECTED (-1)

/**
 * @brief This is the type the library works with. It interacts with it's real world entity by
 * the @c ICharacter interface.
 *
 * Each ai entity has a @c AggroMgr assigned that is updated with each tick. The character also
 * holds a reference to the @c IPathfinder that should be used to move the attached @c ICharacter
 * through the world.
 *
 * A behaviour can be replaced at runtime with @c AI::setBehaviour
 *
 * You can set single @c AI instances to no longer update their state by calling @c AI::setPause
 */
class AI {
	friend class TreeNode;
protected:
	/**
	 * If a node is no longer active, it gets reset. This map holds the state about the resets.
	 */
	typedef std::map<int, bool> ResetStates;
	ResetStates _resetStates;

	/**
	 * Often @c Selector states must be stored to continue in the next step at a particular
	 * position in the behaviour tree. This map is doing exactly this.
	 */
	typedef std::map<int, int> SelectorStates;
	SelectorStates _selectorStates;

	TreeNodePtr _behaviour;
	AggroMgr _aggroList;

	ICharacter& _character;
	IPathfinder& _pathfinder;

	GroupMgr& _groupManager;

	bool _pause;
public:
	/**
	 * @param character The binding to your game entity
	 * @param behaviour The behaviour tree node that is applied to this ai entity
	 * @param pathfinder Some pathfinder implementation
	 * @param groupManager Some group manager implementation
	 */
	AI(ICharacter& character, const TreeNodePtr& behaviour, IPathfinder& pathfinder, GroupMgr& groupManager);
	virtual ~AI();

	/**
	 * @brief Update the behaviour and the aggro values if the entity is not on hold.
	 * @param[in] currentMillis The current milliseconds to update the aggro entries and
	 * time based tasks or conditions.
	 */
	virtual void update(long currentMillis);

	/**
	 * @brief don't update the entity as long as it is paused
	 * @sa isPause()
	 */
	void setPause(bool pause);

	/**
	 * @brief don't update the entity as long as it is paused
	 * @sa setPause()
	 */
	bool isPause() const;

	/**
	 * @brief Get the current behaviour for this ai
	 */
	TreeNodePtr getBehaviour() const;
	/**
	 * @brief Set a new behaviour and returns the old one
	 */
	TreeNodePtr setBehaviour(const TreeNodePtr& newBehaviour);
	/**
	 * @return The real world entity reference
	 */
	ICharacter& getCharacter() const;
	AggroMgr& getAggroMgr();
	IPathfinder& getPathfinder();
	GroupMgr& getGroupMgr();
};

inline TreeNodePtr AI::getBehaviour() const {
	return _behaviour;
}

inline TreeNodePtr AI::setBehaviour(const TreeNodePtr& newBehaviour) {
	TreeNodePtr current = _behaviour;
	_behaviour = newBehaviour;
	return current;
}

inline void AI::setPause(bool pause) {
	_pause = pause;
}

inline bool AI::isPause() const {
	return _pause;
}

inline ICharacter& AI::getCharacter() const {
	return _character;
}

inline AggroMgr& AI::getAggroMgr() {
	return _aggroList;
}

inline IPathfinder& AI::getPathfinder() {
	return _pathfinder;
}

inline GroupMgr& AI::getGroupMgr() {
	return _groupManager;
}

}
