#pragma once

#include <tree/TreeNode.h>
#include <common/AIException.h>
#include <AIRegistry.h>

namespace ai {

/**
 * @brief A node with only one child attached. The result of the attached child is inverted.
 *
 * - If the child returns a TreeNodeStatus::FINISHED, this node will return TreeNodeStatus::FAILED
 * - If the child returns a TreeNodeStatus::FAILED, this node will return TreeNodeStatus::FINISHED
 * - otherwise this node will return a TreeNodeStatus::RUNNING
 */
class Invert: public TreeNode {
public:
	Invert(const std::string& name, const std::string& parameters, const ConditionPtr& condition) :
			TreeNode(name, parameters, condition) {
	}
	virtual ~Invert() {}

	NODE_FACTORY

	TreeNodeStatus execute(AI& entity, long currentMillis) override {
		if (_children.size() != 1) {
			throw AIException("Invert can not have more than one child");
		}

		if (TreeNode::execute(entity, currentMillis) == CANNOTEXECUTE)
			return CANNOTEXECUTE;

		const TreeNodePtr& treeNode = *_children.begin();
		const TreeNodeStatus status = treeNode->execute(entity, currentMillis);
		if (status == FINISHED)
			return FAILED;
		else if (status == FAILED)
			return FINISHED;
		else if (status == EXCEPTION)
			return EXCEPTION;
		else if (status == CANNOTEXECUTE)
			return FINISHED;
		return RUNNING;
	}
};

}
