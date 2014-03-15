#pragma once

#include <string>
#include <vector>
#include <tree/TreeNode.h>

namespace ai {

class LUACondition;
class LUATree;

class LUANode {
private:
	TreeNodePtr _node;
	LUACondition *_condition;
	std::vector<LUANode*> _children;
	LUATree *_tree;
public:
	LUANode(const TreeNodePtr& node, LUATree *tree) :
			_node(node), _condition(nullptr), _tree(tree) {
	}

	inline TreeNodePtr& getTreeNode() {
		return _node;
	}

	inline void setCondition(LUACondition *condition) {
		_condition = condition;
	}

	inline const std::vector<LUANode*>& getChildren() const {
		return _children;
	}

	inline LUACondition* getCondition() const {
		return _condition;
	}

	LUANode* addChild (const std::string& nodeType, const TreeNodeFactoryContext& ctx);
};

}
