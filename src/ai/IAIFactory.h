#pragma once

#include <common/Pointers.h>
#include <string>
#include <vector>

namespace ai {

class TreeNode;
typedef SharedPtr<TreeNode> TreeNodePtr;
typedef std::vector<TreeNodePtr> TreeNodes;

class ICondition;
typedef SharedPtr<ICondition> ConditionPtr;
typedef std::vector<ConditionPtr> Conditions;

struct TreeNodeFactoryContext;
struct ConditionFactoryContext;

class IAIFactory {
public:
	virtual ~IAIFactory() {
	}

	virtual TreeNodePtr createNode(const std::string& nodeType, const TreeNodeFactoryContext& ctx) const = 0;
	virtual ConditionPtr createCondition(const std::string& nodeType, const ConditionFactoryContext& ctx) const = 0;
};

}
