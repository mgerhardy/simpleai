#pragma once

#include <tree/TreeNode.h>

namespace ai {

#define TASK_CLASS_CTOR(TaskName) \
	TaskName(const std::string& name, const std::string& parameters, const ConditionPtr& condition) : \
			ITask(name, parameters, condition)
#define TASK_CLASS_DTOR(TaskName) virtual ~TaskName()

#define TASK_CLASS(TaskName) \
	TASK_CLASS_CTOR(TaskName) {}\
	TASK_CLASS_DTOR(TaskName) {}

class ITask: public TreeNode {
protected:
	TreeNodeStatus execute(AI& entity, long deltaMillis);
public:
	ITask(const std::string& name, const std::string& parameters, const ConditionPtr& condition);
	virtual ~ITask();

	virtual TreeNodeStatus doAction(AI& entity, long deltaMillis) = 0;

	void addChild(const TreeNodePtr& child) override;
};

}
