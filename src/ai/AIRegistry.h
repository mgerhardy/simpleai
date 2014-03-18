#pragma once

#include <IAIFactory.h>
#include <common/IFactoryRegistry.h>

namespace ai {

struct TreeNodeFactoryContext {
	std::string name;
	std::string parameters;
	ConditionPtr condition;
	TreeNodeFactoryContext(const std::string& _name, const std::string& _parameters, const ConditionPtr& _condition) :
			name(_name), parameters(_parameters), condition(_condition) {
	}
};

struct ConditionFactoryContext {
	// Parameters for the condition - can get hand over to the ctor in your factory implementation.
	std::string parameters;
	// Some conditions have child conditions
	Conditions conditions;
	ConditionFactoryContext(const std::string& _parameters) :
		parameters(_parameters) {
	}
};

class ITreeNodeFactory: public IFactory<TreeNode, TreeNodeFactoryContext> {
public:
	virtual ~ITreeNodeFactory() {
	}
	virtual TreeNodePtr create(const TreeNodeFactoryContext *ctx) const = 0;
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

	bool registerConditionFactory(const std::string& nodeType, const IConditionFactory& factory);
	bool unregisterConditionFactory(const std::string& nodeType);

	TreeNodePtr createNode(const std::string& nodeType, const TreeNodeFactoryContext& ctx) const override;
	ConditionPtr createCondition(const std::string& nodeType, const ConditionFactoryContext& ctx) const override;
};

}
