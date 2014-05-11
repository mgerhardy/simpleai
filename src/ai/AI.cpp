#include "AI.h"
#include "common/AIException.h"
#include "tree/TreeNode.h"

namespace ai {

AI::AI(ICharacter& character, const TreeNodePtr& behaviour, IPathfinder& pathfinder, GroupMgr& groupManager) :
		_behaviour(behaviour), _character(character), _pathfinder(pathfinder), _groupManager(groupManager), _pause(false), _time(0L) {
}

AI::~AI() {
}

void AI::update(long deltaMillis) {
	if (isPause())
		return;

	_time += deltaMillis;
	_aggroList.update(deltaMillis);
	_behaviour->execute(*this, deltaMillis);
}

}
