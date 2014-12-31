#pragma once

#include "IAIFactory.h"
#include "common/IFactoryRegistry.h"

namespace ai {

struct TreeNodeFactoryContext {
	std::string name;
	std::string parameters;
	ConditionPtr condition;
	TreeNodeFactoryContext(const std::string& _name, const std::string& _parameters, const ConditionPtr& _condition) :
			name(_name), parameters(_parameters), condition(_condition) {
	}
};

struct FilterFactoryContext {
	// Parameters for the filter - can get hand over to the ctor in your factory implementation.
	std::string parameters;
	FilterFactoryContext(const std::string& _parameters) :
		parameters(_parameters) {
	}
};

struct ConditionFactoryContext {
	// Parameters for the condition - can get hand over to the ctor in your factory implementation.
	std::string parameters;
	// Some conditions have child conditions
	Conditions conditions;
	// The filter condition also has filters
	Filters filters;
	bool filter;
	ConditionFactoryContext(const std::string& _parameters) :
		parameters(_parameters), filter(false) {
	}
};

class ITreeNodeFactory: public IFactory<TreeNode, TreeNodeFactoryContext> {
public:
	virtual ~ITreeNodeFactory() {
	}
	virtual TreeNodePtr create(const TreeNodeFactoryContext *ctx) const = 0;
};

class IFilterFactory: public IFactory<IFilter, FilterFactoryContext> {
public:
	virtual ~IFilterFactory() {
	}
	virtual FilterPtr create(const FilterFactoryContext *ctx) const = 0;
};

class IConditionFactory: public IFactory<ICondition, ConditionFactoryContext> {
public:
	virtual ~IConditionFactory() {
	}
	virtual ConditionPtr create(const ConditionFactoryContext *ctx) const = 0;
};

/**
 * @brief The place to register your @c TreeNode and @c ICondition at
 */
class AIRegistry: public IAIFactory {
protected:
	class TreeNodeFactory: public IFactoryRegistry<std::string, TreeNode, TreeNodeFactoryContext> {
	public:
		TreeNodeFactory();
	};

	TreeNodeFactory _treeNodeFactory;

	class FilterFactory: public IFactoryRegistry<std::string, IFilter, FilterFactoryContext> {
	public:
		FilterFactory();
	};

	FilterFactory _filterFactory;

	class ConditionFactory: public IFactoryRegistry<std::string, ICondition, ConditionFactoryContext> {
	public:
		ConditionFactory();
	};

	ConditionFactory _conditionFactory;
public:
	AIRegistry();
	virtual ~AIRegistry();

	bool registerNodeFactory(const std::string& nodeType, const ITreeNodeFactory& factory);
	bool unregisterNodeFactory(const std::string& nodeType);

	bool registerFilterFactory(const std::string& nodeType, const IFilterFactory& factory);
	bool unregisterFilterFactory(const std::string& nodeType);

	bool registerConditionFactory(const std::string& nodeType, const IConditionFactory& factory);
	bool unregisterConditionFactory(const std::string& nodeType);

	TreeNodePtr createNode(const std::string& nodeType, const TreeNodeFactoryContext& ctx) const override;
	FilterPtr createFilter(const std::string& nodeType, const FilterFactoryContext& ctx) const override;
	ConditionPtr createCondition(const std::string& nodeType, const ConditionFactoryContext& ctx) const override;
};

}
