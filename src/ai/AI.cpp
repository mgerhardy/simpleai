#include <AI.h>
#include <common/AIException.h>
#include <tree/TreeNode.h>

namespace ai {

AI::AI(ICharacter& character, TreeNodePtr behaviour, IPathfinder& pathfinder) :
		_behaviour(behaviour), _character(character), _pathfinder(pathfinder), _pause(false) {
}

AI::~AI() {
}

void AI::update(long currentMillis) {
	_aggroList.update(currentMillis);

	if (isPause())
		return;

	_behaviour->execute(*this, currentMillis);
}

}
