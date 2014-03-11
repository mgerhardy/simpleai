#pragma once

#include "../GameEntity.h"
#include <tree/ITask.h>

namespace ai {
namespace example {

class Move: public ITask {
private:
	inline ai::AIPosition getRandomDirection() const {
		// yes, not really random due to the modulo, but... oh well
		const int r = rand() % 4;
		if (r == 0) {
			return ai::AIPosition(1, 0);
		} else if (r == 1) {
			return ai::AIPosition(-1, 0);
		} else if (r == 2) {
			return ai::AIPosition(0, 1);
		}
		return ai::AIPosition(0, -1);
	}

public:
	TASK_CLASS(Move);
	NODE_FACTORY

	TreeNodeStatus doAction(AI& entity) {
		GameEntity& chr = static_cast<GameEntity&>(entity.getCharacter());
		std::list<ai::AIPosition>& _route = chr.getRoute();
		if (!_route.empty()) {
			const ai::AIPosition& pos = *_route.begin();
			chr.setPosition(pos);
			_route.erase(_route.begin());
			return RUNNING;
		}
		AIPosition position = chr.getPosition();
		for (int i = 0; i < 10; ++i) {
			position += getRandomDirection();
		}
		if (entity.getPathfinder().move(entity, position, _route) == SUCCESSFUL)
			return RUNNING;
		return FAILED;
	}
};

TreeNodePtr Move::Factory::create(const TreeNodeFactoryContext *ctx) const {
	return TreeNodePtr(new Move(ctx->name));
}

Move::Factory Move::FACTORY;

}
}
