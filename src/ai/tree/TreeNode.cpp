#include "tree/TreeNode.h"
#include "AI.h"

namespace ai {

int TreeNode::_nextId = 0;

TreeNode::TreeNode(const std::string& name, const std::string& parameters, const ConditionPtr& condition) :
		_id(_nextId++), _name(name), _parameters(parameters), _condition(condition) {
}

TreeNode::~TreeNode() {
}

TreeNodeStatus TreeNode::execute(AI& entity, long) {
	if (!_condition->evaluate(entity)) {
		return state(entity, CANNOTEXECUTE);
	}

	setLastExecMillis(entity);
	return state(entity, FINISHED);
}

void TreeNode::resetState(AI& entity) {
	for (TreeNodes::iterator i = _children.begin(); i != _children.end(); ++i)
		(*i)->resetState(entity);
}

void TreeNode::getRunningChildren(const AI& /*entity*/, std::vector<bool>& active) const {
	for (TreeNodes::const_iterator i = _children.begin(); i != _children.end(); ++i)
		active.push_back(false);
}

inline void TreeNode::setLastExecMillis(AI& entity) {
	if (!entity._debuggingActive)
		return;
	entity._lastExecMillis[getId()] = entity._time;
}

int TreeNode::getSelectorState(const AI& entity) const {
	AI::SelectorStates::const_iterator i = entity._selectorStates.find(getId());
	if (i == entity._selectorStates.end())
		return NOTHING_SELECTED;
	return i->second;
}

void TreeNode::setSelectorState(AI& entity, int selected) {
	entity._selectorStates[getId()] = selected;
}

TreeNodeStatus TreeNode::state(AI& entity, TreeNodeStatus treeNodeState) {
	entity._lastStatus[getId()] = treeNodeState;
	return treeNodeState;
}

long TreeNode::getLastExecMillis(const AI& entity) const {
	if (!entity._debuggingActive)
		return -1L;
	AI::LastExecMap::const_iterator i = entity._lastExecMillis.find(getId());
	if (i == entity._lastExecMillis.end())
		return -1L;
	return i->second;
}

TreeNodeStatus TreeNode::getLastStatus(const AI& entity) const {
	AI::NodeStates::const_iterator i = entity._lastStatus.find(getId());
	if (i == entity._lastStatus.end())
		return UNKNOWN;
	return i->second;
}

std::ostream& TreeNode::print(std::ostream& stream, int level) const {
	for (int i = 0; i < level; ++i) {
		stream << '\t';
	}
	if (_condition) {
		stream << "if (";
		_condition->print(stream, level);
		stream << ") => ";
	}
	stream << _name;
	stream << "(";
	if (!_parameters.empty()) {
		stream << "\"" << _parameters << "\"";
	}
	stream << ")";
	if (!_children.empty()) {
		stream << " {" << std::endl;
		for (TreeNodes::const_iterator i = _children.begin(); i != _children.end(); ++i) {
			(*i)->print(stream, level + 1);
		}
		for (int i = 0; i < level; ++i) {
			stream << '\t';
		}
		stream << "}";
	}
	stream << std::endl;
	return stream;
}

}
