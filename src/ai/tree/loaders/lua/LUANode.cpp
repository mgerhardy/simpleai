#include "LUATree.h"
#include "LUANode.h"

namespace ai {

LUANode* LUANode::addChild(const std::string& nodeType, const TreeNodeFactoryContext& ctx) {
	const TreeNodePtr& child = _tree->getAIFactory().createNode(nodeType, ctx);
	LUANode *node = new LUANode(child, _tree);
	_children.push_back(node);
	_node->addChild(child);
	return node;
}

}
