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

	/**
	 * @brief Allocates a new @c TreeNode for the given @c type. The @c type must be registered in the @c AIRegistry for this to work.
	 */
	virtual TreeNodePtr createNode(const std::string& type, const TreeNodeFactoryContext& ctx) const = 0;
	/**
	 * @brief Allocates a new @c IFilter for the given @c type. The @c type must be registered in the @c AIRegistry for this to work.
	 */
	virtual FilterPtr createFilter(const std::string& type, const FilterFactoryContext& ctx) const = 0;
	/**
	 * @brief Allocates a new @c ICondition for the given @c type. The @c type must be registered in the @c AIRegistry for this to work.
	 */
	virtual ConditionPtr createCondition(const std::string& type, const ConditionFactoryContext& ctx) const = 0;
};

}
