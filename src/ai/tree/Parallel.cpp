#include "tree/Parallel.h"
#include "AI.h"

namespace ai {

void Parallel::getRunningChildren(const AI& entity, std::vector<bool>& active) const {
	for (TreeNodes::const_iterator i = _children.begin(); i != _children.end(); ++i) {
		active.push_back(!(*i)->getResetSinceLastExec(entity));
	}
}

TreeNodeStatus Parallel::execute(AI& entity, long deltaMillis) {
	if (Selector::execute(entity, deltaMillis) == CANNOTEXECUTE)
		return CANNOTEXECUTE;

	bool totalStatus = false;
	const std::size_t size = _children.size();
	for (std::size_t i = 0; i < size; ++i) {
		TreeNodePtr& child = _children[i];
		const bool isActive = child->execute(entity, deltaMillis) == RUNNING;
		if (!isActive) {
			child->resetState(entity);
		}
		totalStatus |= isActive;
	}

	if (!totalStatus) {
		resetState(entity);
	}
	return state(entity, totalStatus ? RUNNING : FINISHED);
}

NODE_FACTORY_IMPL(Parallel)

}
