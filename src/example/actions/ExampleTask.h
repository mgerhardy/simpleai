#pragma once

#include <AI.h>
#include <tree/ITask.h>

namespace ai {
namespace example {

class ExampleTask: public ITask {
public:
	TASK_CLASS(ExampleTask);
	NODE_FACTORY

	TreeNodeStatus doAction(AI& entity) {
		//ICharacter& chr = entity.getCharacter();
		return FINISHED;
	}
};

TreeNodePtr ExampleTask::Factory::create(const TreeNodeFactoryContext *ctx) const {
	return TreeNodePtr(new ExampleTask(ctx->name));
}

ExampleTask::Factory ExampleTask::FACTORY;

}
}
