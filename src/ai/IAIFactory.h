#pragma once

#include <memory>
#include <string>
#include <vector>
#include <list>

namespace ai {

class TreeNode;
typedef std::shared_ptr<TreeNode> TreeNodePtr;
typedef std::vector<TreeNodePtr> TreeNodes;

class IFilter;
typedef std::shared_ptr<IFilter> FilterPtr;
typedef std::list<FilterPtr> Filters;

class ICondition;
typedef std::shared_ptr<ICondition> ConditionPtr;
typedef std::vector<ConditionPtr> Conditions;

struct TreeNodeFactoryContext;
struct ConditionFactoryContext;
struct FilterFactoryContext;

class IAIFactory {
public:
	virtual ~IAIFactory() {
	}

	virtual TreeNodePtr createNode(const std::string& nodeType, const TreeNodeFactoryContext& ctx) const = 0;
	virtual FilterPtr createFilter(const std::string& nodeType, const FilterFactoryContext& ctx) const = 0;
	virtual ConditionPtr createCondition(const std::string& nodeType, const ConditionFactoryContext& ctx) const = 0;
};

}
