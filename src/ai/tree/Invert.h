#pragma once

#include "tree/TreeNode.h"
#include "common/AIException.h"
#include "AIRegistry.h"

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
	NODE_CLASS(Invert)

	TreeNodeStatus execute(AI& entity, long deltaMillis) override {
		if (_children.size() != 1) {
			throw AIException("Invert can not have more than one child");
		}

		if (TreeNode::execute(entity, deltaMillis) == CANNOTEXECUTE)
			return state(CANNOTEXECUTE);

		const TreeNodePtr& treeNode = *_children.begin();
		const TreeNodeStatus status = treeNode->execute(entity, deltaMillis);
		if (status == FINISHED)
			return state(FAILED);
		else if (status == FAILED)
			return state(FINISHED);
		else if (status == EXCEPTION)
			return state(EXCEPTION);
		else if (status == CANNOTEXECUTE)
			return state(FINISHED);
		return state(RUNNING);
	}
};

}
