#include "AI.h"
#include "ICharacter.h"
#include "tree/TreeNode.h"
#include "zone/Zone.h"

namespace ai {

AI::AI(const TreeNodePtr& behaviour) :
		_behaviour(behaviour), _pause(false), _debuggingActive(false), _time(0L), _zone(nullptr), _reset(false) {
}

AI::~AI() {
}

Vector3f AI::getGroupPosition(GroupId id) const {
	Zone* local = _zone;
	if (local == nullptr || id < 0)
		return Vector3f::INFINITE;

	return local->getGroupMgr().getPosition(id);
}

Vector3f AI::getGroupLeaderPosition(GroupId id) const {
	Zone* local = _zone;
	if (local == nullptr)
		return Vector3f::INFINITE;

	const AIPtr& groupLeader = local->getGroupMgr().getLeader(id);
	if (groupLeader)
		return groupLeader->getCharacter()->getPosition();
	return Vector3f::INFINITE;
}

CharacterId AI::getId() const {
	if (!_character)
		return NOTHING_SELECTED;
	return _character->getId();
}

void AI::update(long dt, bool debuggingActive) {
	if (isPause())
		return;

	if (_character)
		_character->update(dt, debuggingActive);

	if (_reset) {
		// safe to do it like this, because update is not called from multiple threads
		_reset = false;
		_lastStatus.clear();
		_lastExecMillis.clear();
		_filteredEntities.clear();
		_selectorStates.clear();
	}

	_debuggingActive = debuggingActive;
	_time += dt;
	_aggroList.update(dt);
}

}
