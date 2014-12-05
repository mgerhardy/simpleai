#pragma once

#include <vector>
#include <string>
#include <memory>
#include "common/IPrintable.h"
#include "conditions/ICondition.h"
#include "conditions/True.h"

namespace ai {

class TreeNode;
typedef std::shared_ptr<TreeNode> TreeNodePtr;
typedef std::vector<TreeNodePtr> TreeNodes;

enum TreeNodeStatus {
	UNKNOWN,
	/**
	 * Not every condition is met in order to run this node
	 * In general this means that the attached condition has to evaluate to @c true
	 */
	CANNOTEXECUTE,
	/**
	 * This behavior is still running and thus can block others
	 */
	RUNNING,
	/**
	 * The behavior ran and terminated without any problems.
	 */
	FINISHED,
	/**
	 * Controlled failure
	 */
	FAILED,
	/**
	 * Unexpected failure while executing the the node's action
	 */
	EXCEPTION,

	MAX_TREENODESTATUS
};

#define NODE_FACTORY \
	class Factory: public ITreeNodeFactory { \
	public: \
		TreeNodePtr create (const TreeNodeFactoryContext *ctx) const; \
	}; \
	static Factory FACTORY;

#define NODE_FACTORY_IMPL(NodeName) \
	TreeNodePtr NodeName::Factory::create(const TreeNodeFactoryContext *ctx) const { \
		return TreeNodePtr(new NodeName(ctx->name, ctx->parameters, ctx->condition)); \
	} \
	NodeName::Factory NodeName::FACTORY;

#define NODE_CLASS(NodeName) \
	NodeName(const std::string& name, const std::string& parameters, const ConditionPtr& condition) : \
		TreeNode(name, parameters, condition) { \
	} \
	virtual ~NodeName() { \
	} \
	\
	NODE_FACTORY

/**
 * @brief The base class for all behaviour tree actions.
 *
 * @c TreeNode::execute is triggered with each @c AI::update.
 * Also the attached @c ICondition is evaluated here. States are stored on the
 * connected @c AI instance. Don't store states on tree nodes, because they can
 * be reused for multiple @c AI instances. Always use the @c AI or @c ICharacter
 * to store your state!
 */
class TreeNode : public IPrintable {
protected:
	static int _nextId;
	int _id;
	TreeNodes _children;
	std::string _name;
	std::string _parameters;
	ConditionPtr _condition;

	void setResetSinceLastExec(AI& entity, bool status);
	TreeNodeStatus state(AI& entity, TreeNodeStatus treeNodeState);
	int getSelectorState(const AI& entity) const;
	void setSelectorState(AI& entity, int selected);
	void setLastExecMillis(AI& entity);

public:
	TreeNode(const std::string& name, const std::string& parameters, const ConditionPtr& condition);
	virtual ~TreeNode();
	/**
	 * @brief Return the unique id for this node.
	 * @return unique id
	 */
	int getId() const;

	/**
	 * @brief Returns the time in milliseconds when this node was last run. This is only updated if @c #execute() was called
	 */
	const std::string& getName() const;
	const ConditionPtr& getCondition() const;
	void setCondition(const ConditionPtr& condition);
	const TreeNodes& getChildren() const;
	bool getResetSinceLastExec(const AI& entity) const;

	/**
	 * @brief Get the state of all child nodes for the given entity
	 * @param[in] entity The entity to get the child node states for
	 */
	virtual void getRunningChildren(const AI& entity, std::vector<bool>& active) const;
	long getLastExecMillis(const AI& ai) const;
	TreeNodeStatus getLastStatus(const AI& ai) const;

	virtual TreeNodeStatus execute(AI& entity, long deltaMillis);

	/**
	 * @brief Reset the states in the node and also in the entity
	 */
	virtual void resetState(AI& entity);

	virtual void addChild(const TreeNodePtr& child);

	std::ostream& print(std::ostream& stream, int level) const override;
};

inline int TreeNode::getId() const {
	return _id;
}

inline const std::string& TreeNode::getName() const {
	return _name;
}

inline const ConditionPtr& TreeNode::getCondition() const {
	return _condition;
}

inline void TreeNode::setCondition(const ConditionPtr& condition) {
	_condition = condition;
}

inline const TreeNodes& TreeNode::getChildren() const {
	return _children;
}

inline void TreeNode::addChild(const TreeNodePtr& child) {
	_children.push_back(child);
}

}
