#include "AI.h"
#include "common/AIException.h"
#include "tree/TreeNode.h"

namespace ai {

AI::AI(ICharacter& character, const TreeNodePtr& behaviour, GroupMgr& groupManager) :
		_behaviour(behaviour), _character(character), _groupManager(groupManager), _pause(false), _debuggingActive(false), _time(0L) {
}

AI::~AI() {
}

void AI::update(long dt, bool debuggingActive) {
	if (isPause())
		return;

	_debuggingActive = debuggingActive;
	_time += dt;
	_aggroList.update(dt);
	_behaviour->execute(*this, dt);
}

}
