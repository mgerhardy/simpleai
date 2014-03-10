#pragma once

#include <map>

#include <aggro/AggroMgr.h>
#include <tree/loaders/ITreeLoader.h>
#include <ICharacter.h>
#include <pathfinding/IPathfinder.h>

namespace ai {

#define NOTHING_SELECTED (-1)

class AI {
	friend class TreeNode;
protected:
	typedef std::map<int, bool> ResetStates;
	ResetStates _resetStates;

	typedef std::map<int, int> SelectorStates;
	SelectorStates _selectorStates;

	TreeNodePtr _behaviour;
	AggroMgr _aggroList;

	ICharacter& _character;
	IPathfinder& _pathfinder;

	bool _pause;
public:
	AI(ICharacter& character, TreeNodePtr behaviour, IPathfinder& pathfinder);
	virtual ~AI();

	/**
	 * @brief Update the behaviour and the aggro values if the entity is not on hold.
	 * @param[in] currentMillis The current milliseconds to update the aggro entries and
	 * time based tasks or conditions.
	 */
	virtual void update(long currentMillis);

	void setPause(bool pause);

	/**
	 * @brief don't update the entity as long as it is paused
	 */
	bool isPause() const;

	/**
	 * @brief Get the current behaviour for this ai
	 */
	TreeNodePtr getBehaviour() const;
	/**
	 * @brief Set a new behaviour and returns the old one
	 */
	TreeNodePtr setBehaviour(TreeNodePtr newBehaviour);
	ICharacter& getCharacter() const;
	AggroMgr& getAggroMgr();
	IPathfinder& getPathfinder();
};

inline TreeNodePtr AI::getBehaviour() const {
	return _behaviour;
}

inline TreeNodePtr AI::setBehaviour(TreeNodePtr newBehaviour) {
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

}
