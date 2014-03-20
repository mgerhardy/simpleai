#include <tree/TreeNode.h>
#include <AI.h>

namespace ai {

int TreeNode::_nextId = 0;

TreeNode::TreeNode(const std::string& name, const std::string& parameters, const ConditionPtr& condition) :
		_id(_nextId++), _name(name), _parameters(parameters), _condition(condition), _lastExecMillis(-1L), _time(0L) {
}

TreeNode::~TreeNode() {
}

TreeNodeStatus TreeNode::execute(AI& entity, long deltaMillis) {
	_time += deltaMillis;
	if (!_condition->evaluate(entity))
		return CANNOTEXECUTE;

	_lastExecMillis = _time;
	return FINISHED;
}

void TreeNode::resetState(AI& entity) {
	if (getResetSinceLastExec(entity))
		return;

	for (TreeNodes::iterator i = _children.begin(); i != _children.end(); ++i)
		(*i)->resetState(entity);

	setResetSinceLastExec(entity, true);
}

void TreeNode::getChildrenState(const AI& entity, std::vector<bool>& active) const {
	for (TreeNodes::const_iterator i = _children.begin(); i != _children.end(); ++i)
		active.push_back(false);
}

bool TreeNode::getResetSinceLastExec(const AI& entity) const {
	AI::ResetStates::const_iterator i = entity._resetStates.find(getId());
	if (i == entity._resetStates.end())
		return false;
	return i->second;
}

void TreeNode::setResetSinceLastExec(AI& entity, bool status) {
	entity._resetStates[getId()] = status;
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

}
