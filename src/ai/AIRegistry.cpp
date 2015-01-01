#include "AIRegistry.h"
#include "tree/TreeNode.h"
#include "conditions/ICondition.h"
#include "tree/Invert.h"
#include "tree/Idle.h"
#include "tree/Print.h"
#include "tree/Wander.h"
#include "tree/Parallel.h"
#include "tree/PrioritySelector.h"
#include "tree/Sequence.h"
#include "tree/group/FollowGroup.h"
#include "tree/group/FleeGroup.h"
#include "conditions/And.h"
#include "conditions/False.h"
#include "conditions/HasEnemies.h"
#include "conditions/Not.h"
#include "conditions/Filter.h"
#include "conditions/Or.h"
#include "conditions/True.h"
#include "conditions/IsInGroup.h"
#include "conditions/IsGroupLeader.h"
#include "conditions/IsCloseToGroup.h"
#include "filter/IFilter.h"
#include "filter/SelectEmpty.h"
#include "filter/SelectHighestAggro.h"
#include "filter/SelectGroupMembers.h"

namespace ai {

#define R(Name) registerFactory(#Name, Name::FACTORY);

AIRegistry::ConditionFactory::ConditionFactory() {
	R(And);
	R(False);
	R(HasEnemies);
	R(Not);
	R(Or);
	R(True);
	R(Filter);
	R(IsGroupLeader);
	R(IsInGroup);
	R(IsCloseToGroup);
}

AIRegistry::FilterFactory::FilterFactory() {
	R(SelectEmpty);
	R(SelectGroupMembers);
	R(SelectHighestAggro);
}

AIRegistry::TreeNodeFactory::TreeNodeFactory() {
	R(Invert);
	R(Parallel);
	R(PrioritySelector);
	R(Sequence);
	R(Print);
	R(Wander);
	R(FollowGroup);
	R(FleeGroup);
	R(Idle);
}

#undef R

AIRegistry::AIRegistry() :
		IAIFactory() {
}

AIRegistry::~AIRegistry() {
}

TreeNodePtr AIRegistry::createNode(const std::string& nodeType, const TreeNodeFactoryContext& ctx) const {
	return _treeNodeFactory.create(nodeType, &ctx);
}

FilterPtr AIRegistry::createFilter(const std::string& nodeType, const FilterFactoryContext& ctx) const {
	return _filterFactory.create(nodeType, &ctx);
}

bool AIRegistry::registerNodeFactory(const std::string& nodeType, const ITreeNodeFactory& factory) {
	return _treeNodeFactory.registerFactory(nodeType, factory);
}

bool AIRegistry::registerFilterFactory(const std::string& nodeType, const IFilterFactory& factory) {
	return _filterFactory.registerFactory(nodeType, factory);
}

bool AIRegistry::registerConditionFactory(const std::string& nodeType, const IConditionFactory& factory) {
	return _conditionFactory.registerFactory(nodeType, factory);
}

bool AIRegistry::unregisterNodeFactory(const std::string& nodeType) {
	return _treeNodeFactory.unregisterFactory(nodeType);
}

bool AIRegistry::unregisterFilterFactory(const std::string& nodeType) {
	return _filterFactory.unregisterFactory(nodeType);
}

bool AIRegistry::unregisterConditionFactory(const std::string& nodeType) {
	return _conditionFactory.unregisterFactory(nodeType);
}

ConditionPtr AIRegistry::createCondition(const std::string& nodeType, const ConditionFactoryContext& ctx) const {
	return _conditionFactory.create(nodeType, &ctx);
}

}
