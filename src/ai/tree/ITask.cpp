#include <tree/ITask.h>
#include <common/AIException.h>

namespace ai {

ITask::ITask(const std::string& name, ConditionPtr condition) :
		TreeNode(name, condition) {
	init();
}

ITask::ITask(const std::string& name) :
		TreeNode(name) {
	init();
}

ITask::~ITask() {
}

TreeNodeStatus ITask::execute(AI& entity, long currentMillis) {
	if (TreeNode::execute(entity, currentMillis) == CANNOTEXECUTE)
		return CANNOTEXECUTE;

	try {
		return doAction(entity);
	} catch (std::exception& e) {
		return EXCEPTION;
	}
}

void ITask::addChild(TreeNodePtr& child) {
	throw AIException("An action node may not have a child");
}

}
