#include <tree/PrioritySelector.h>
#include <AI.h>

namespace ai {

TreeNodeStatus PrioritySelector::execute(AI& entity, long deltaMillis) {
	if (Selector::execute(entity, deltaMillis) == CANNOTEXECUTE)
		return CANNOTEXECUTE;

	TreeNodes::iterator i = _children.begin();
	for (; i != _children.end(); ++i) {
		const TreeNodePtr& child = *i;
		const TreeNodeStatus result = child->execute(entity, deltaMillis);
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

NODE_FACTORY_IMPL(PrioritySelector)

}
