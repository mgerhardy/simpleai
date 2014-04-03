#include "tree/PrioritySelector.h"
#include "AI.h"

namespace ai {

TreeNodeStatus PrioritySelector::execute(AI& entity, long deltaMillis) {
	setSelectorState(entity, NOTHING_SELECTED);
	if (Selector::execute(entity, deltaMillis) == CANNOTEXECUTE)
		return state(CANNOTEXECUTE);

	const std::size_t size = _children.size();
	for (std::size_t i = 0; i < size; i++) {
		const TreeNodePtr& child = _children[i];
		const TreeNodeStatus result = child->execute(entity, deltaMillis);
		if (result == RUNNING)
			setSelectorState(entity, static_cast<int>(i));
		else
			child->resetState(entity);

		if (result == CANNOTEXECUTE)
			continue;

		for (++i; i < size; ++i) {
			_children[i]->resetState(entity);
		}

		return state(result);
	}

	return state(FINISHED);
}

NODE_FACTORY_IMPL(PrioritySelector)

}
