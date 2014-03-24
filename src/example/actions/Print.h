#pragma once

#include <AI.h>
#include <tree/ITask.h>
#include <iostream>

namespace ai {
namespace example {

class Print: public ITask {
public:
	TASK_CLASS(Print);
	NODE_FACTORY

	TreeNodeStatus doAction(AI& entity) {
		std::cout << _parameters << std::endl;
		return FINISHED;
	}
};

NODE_FACTORY_IMPL(Print)

}
}
