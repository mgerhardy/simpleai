#include <tree/ITask.h>
#include <common/AIException.h>

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
		return doAction(entity);
	} catch (std::exception& e) {
		return EXCEPTION;
	}
}

void ITask::addChild(const TreeNodePtr& child) {
	throw AIException("An action node may not have a child");
}

}
