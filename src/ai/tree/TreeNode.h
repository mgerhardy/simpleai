#pragma once

#include <vector>
#include <string>
#include <common/Pointers.h>
#include <common/Compiler.h>
#include <conditions/ICondition.h>
#include <conditions/True.h>

namespace ai {

class TreeNode;
typedef SharedPtr<TreeNode> TreeNodePtr;
typedef std::vector<TreeNodePtr> TreeNodes;

enum TreeNodeStatus {
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
	EXCEPTION
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

class TreeNode {
protected:
	static int _nextId;
	int _id;
	TreeNodes _children;
	std::string _name;
	std::string _parameters;
	ConditionPtr _condition;
	long _lastExecMillis;
	long _time;

	bool getResetSinceLastExec(const AI& entity) const;
	void setResetSinceLastExec(AI& entity, bool status);

	int getSelectorState(const AI& entity) const;
	void setSelectorState(AI& entity, int selected);

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
	long getLastExecMillis() const;
	const std::string& getName() const;
	const ConditionPtr& getCondition() const;
	void setCondition(const ConditionPtr& condition);
	const TreeNodes& getChildren() const;

	/**
	 * @brief Get the state of all child nodes for the given entity
	 * @param[in] entity The entity to get the child node states for
	 */
	virtual void getChildrenState(const AI& entity, std::vector<bool>& active) const;

	virtual TreeNodeStatus execute(AI& entity, long currentMillis);

	/**
	 * @brief Reset the states in the node and also in the entity
	 */
	virtual void resetState(AI& entity);

	virtual void addChild(const TreeNodePtr& child);

	friend inline std::ostream& operator<<(std::ostream& stream, const TreeNode& node) {
		stream << node._name;
		const TreeNodes& children = node._children;
		const ConditionPtr& condition = node._condition;
		if (condition) {
			stream << "(";
			stream << *condition.get();
			stream << ")";
		}
		stream << "[";
		stream << node._parameters;
		stream << "]";
		stream << "{";
		for (TreeNodes::const_iterator i = children.begin(); i != children.end(); ++i) {
			stream << *i;
		}
		stream << "}";
	    return stream;
	}
};

inline int TreeNode::getId() const {
	return _id;
}

inline const std::string& TreeNode::getName() const {
	return _name;
}

inline long TreeNode::getLastExecMillis() const {
	return _lastExecMillis;
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
