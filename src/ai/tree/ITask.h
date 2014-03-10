#pragma once

#include <tree/TreeNode.h>

namespace ai {

#define TASK_CLASS(TaskName) \
	TaskName(const std::string& name, ConditionPtr condition) : \
			ITask(name, condition) { \
	} \
	TaskName(const std::string& name) : \
			ITask(name) { \
	} \
	virtual ~TaskName() { \
	}

class ITask: public TreeNode {
protected:
	TreeNodeStatus execute(AI& entity, long currentMillis);

	virtual void init() {}
public:
	ITask(const std::string& name, ConditionPtr condition);
	ITask(const std::string& name);
	virtual ~ITask();

	virtual TreeNodeStatus doAction(AI& entity) = 0;

	void addChild(TreeNodePtr& child) override;
};

}
