#include "AI.h"
#include "tree/TreeNode.h"
#include "server/Zone.h"

namespace ai {

AI::AI(ICharacter& character, const TreeNodePtr& behaviour) :
		_behaviour(behaviour), _character(character), _pause(false), _debuggingActive(false), _time(0L), _zone(nullptr), _reset(false) {
}

AI::~AI() {
	Zone* local = _zone;
	if (local != nullptr) {
		local->getGroupMgr().removeFromAllGroups(&_character);
	}
}

Vector3f AI::getGroupPosition(GroupId id) const {
	Zone* local = _zone;
	if (local == nullptr)
		return Vector3f::INFINITE;

	return local->getGroupMgr().getPosition(id);
}

void AI::update(long dt, bool debuggingActive) {
	if (isPause())
		return;

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
	_behaviour->execute(*this, dt);
}

}
