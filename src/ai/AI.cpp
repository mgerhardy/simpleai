#include "AI.h"
#include "common/AIException.h"
#include "tree/TreeNode.h"

namespace ai {

AI::AI(ICharacter& character, const TreeNodePtr& behaviour, GroupMgr& groupManager) :
		_behaviour(behaviour), _character(character), _groupManager(groupManager), _pause(false), _debuggingActive(false), _time(0L), _reset(false) {
}

AI::~AI() {
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
