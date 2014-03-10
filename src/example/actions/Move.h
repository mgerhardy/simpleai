#pragma once

#include <AI.h>
#include <tree/ITask.h>

namespace ai {
namespace example {

class Move: public ITask {
protected:
	std::list<AIPosition> _route;
public:
	TASK_CLASS(Move);
	NODE_FACTORY

	TreeNodeStatus doAction(AI& entity) {
		ICharacter& chr = entity.getCharacter();
		if (!_route.empty()) {
			const ai::AIPosition& pos = *_route.begin();
			chr.setPosition(pos);
			_route.erase(_route.begin());
			return RUNNING;
		}
		AIPosition position = chr.getPosition();
		position += AIPosition(1, 1);
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
