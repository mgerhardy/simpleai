#pragma once

#include "tree/ITask.h"
#include "common/String.h"
#include "common/Math.h"

namespace ai {

/**
 * @brief Wander forward in the current direction
 *
 * @sa ai::movement::Wander
 */
class Wander: public ITask {
protected:
	float _rotation;
public:
	TASK_CLASS_CTOR(Wander) {
		if (_parameters.empty()) {
			_rotation = 8.0f * static_cast<float>(M_PI);
		} else {
			_rotation = std::stof(_parameters);
		}
	}
	NODE_FACTORY

	TreeNodeStatus doAction(AI& entity, long deltaMillis) override;
};

}
