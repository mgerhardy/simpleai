#pragma once

#include <tree/TreeNode.h>

namespace ai {

#define TASK_CLASS(TaskName) \
	TaskName(const std::string& name, const std::string& parameters, const ConditionPtr& condition) : \
			ITask(name, parameters, condition) { \
	} \
	virtual ~TaskName() { \
	}

class ITask: public TreeNode {
protected:
	TreeNodeStatus execute(AI& entity, long currentMillis);
public:
	ITask(const std::string& name, const std::string& parameters, const ConditionPtr& condition = True::get());
	virtual ~ITask();

	virtual TreeNodeStatus doAction(AI& entity) = 0;

	void addChild(TreeNodePtr& child) override;
};

}
