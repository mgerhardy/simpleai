#pragma once

#include "../GameEntity.h"
#include <tree/ITask.h>

namespace ai {
namespace example {

class Move: public ITask {
private:
	inline ai::Vector3f getRandomDirection() const {
		// yes, not really random due to the modulo, but... oh well
		const int r = rand() % 4;
		if (r == 0) {
			return ai::Vector3f(1, 0);
		} else if (r == 1) {
			return ai::Vector3f(-1, 0);
		} else if (r == 2) {
			return ai::Vector3f(0, 1);
		}
		return ai::Vector3f(0, -1);
	}

public:
	TASK_CLASS(Move);
	NODE_FACTORY

	TreeNodeStatus doAction(AI& entity) {
		GameEntity& chr = character_cast<GameEntity>(entity.getCharacter());
		std::list<ai::MoveVector>& route = chr.getRoute();
		if (!route.empty()) {
			return RUNNING;
		}
		Vector3f position = chr.getPosition();
		for (int i = 0; i < 10; ++i) {
			position += getRandomDirection();
		}
		if (entity.getPathfinder().move(entity, position, route) == SUCCESSFUL)
			return RUNNING;
		return FAILED;
	}
};

NODE_FACTORY_IMPL(Move)

}
}
