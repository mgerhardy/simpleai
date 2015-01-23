#pragma once

#include "tree/ITask.h"
#include "common/String.h"
#include "common/Math.h"
#include "movement/Steering.h"

namespace ai {

/**
 * @brief Wander forward in the current direction
 *
 * @sa ai::movement::Wander
 */
class Wander: public ITask {
protected:
	movement::Wander _w;
public:
	TASK_CLASS_CTOR(Wander), _w(_parameters) {
	}
	NODE_FACTORY

	TreeNodeStatus doAction(AI& entity, long deltaMillis) override;
};

}
