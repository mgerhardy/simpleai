#include <tree/PrioritySelector.h>
#include <AI.h>

namespace ai {

TreeNodeStatus PrioritySelector::execute(AI& entity, long currentMillis) {
	if (Selector::execute(entity, currentMillis) == CANNOTEXECUTE)
		return CANNOTEXECUTE;

	TreeNodes::iterator i = _children.begin();
	for (; i != _children.end(); ++i) {
		const TreeNodePtr& child = *i;
		const TreeNodeStatus result = child->execute(entity, currentMillis);
		if (result != RUNNING)
			child->resetState(entity);

		if (result == CANNOTEXECUTE)
			continue;

		for (++i; i != _children.end(); ++i)
			(*i)->resetState(entity);

		return result;
	}

	return FINISHED;
}

TreeNodePtr PrioritySelector::Factory::create(const TreeNodeFactoryContext *ctx) const {
	return TreeNodePtr(new PrioritySelector(ctx->name, ctx->parameters, ctx->condition));
}

PrioritySelector::Factory PrioritySelector::FACTORY;

}
