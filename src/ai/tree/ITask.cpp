#include "tree/ITask.h"
#include "common/Types.h"

namespace ai {

ITask::ITask(const std::string& name, const std::string& parameters, const ConditionPtr& condition) :
		TreeNode(name, parameters, condition) {
}

ITask::~ITask() {
}

TreeNodeStatus ITask::execute(AI& entity, long deltaMillis) {
	if (TreeNode::execute(entity, deltaMillis) == CANNOTEXECUTE)
		return CANNOTEXECUTE;

	try {
		return state(entity, doAction(entity, deltaMillis));
	} catch (...) {
		return state(entity, EXCEPTION);
	}
}

void ITask::addChild(const TreeNodePtr& /*child*/) {
	ai_assert(false, "An action node may not have a child");
}

}
