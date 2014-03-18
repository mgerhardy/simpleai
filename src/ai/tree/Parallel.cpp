#include <tree/Parallel.h>
#include <AI.h>

namespace ai {

void Parallel::getChildrenState(const AI& entity, std::vector<bool>& active) const {
	for (TreeNodes::const_iterator i = _children.begin(); i != _children.end(); ++i) {
		active.push_back(!getResetSinceLastRun(entity));
	}
}

TreeNodeStatus Parallel::execute(AI& entity, long currentMillis) {
	if (Selector::execute(entity, currentMillis) == CANNOTEXECUTE)
		return CANNOTEXECUTE;

	bool totalStatus = false;
	const std::size_t size = _children.size();
	for (std::size_t i = 0; i < size; ++i) {
		TreeNodePtr& child = _children[i];
		const bool isActive = child->execute(entity, currentMillis) == RUNNING;
		if (!isActive) {
			child->resetState(entity);
		}
		totalStatus |= isActive;
	}

	return totalStatus ? RUNNING : FINISHED;
}

NODE_FACTORY_IMPL(Parallel)

}
