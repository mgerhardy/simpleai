#include <tree/Sequence.h>
#include <AI.h>

namespace ai {

TreeNodeStatus Sequence::execute(AI& entity, long currentMillis) {
	Selector::execute(entity, currentMillis);

	TreeNodeStatus result = FINISHED;
	const int progress = std::max(0, getSelectorState(entity));

	const std::size_t size = _children.size();
	for (std::size_t i = progress; i < size; i++) {
		TreeNodePtr& child = _children[i];

		result = child->execute(entity, currentMillis);

		if (result == RUNNING) {
			setSelectorState(entity, i);
			break;
		} else if (result == CANNOTEXECUTE) {
			resetState(entity);
			continue;
		} else if (result == FAILED || result == EXCEPTION) {
			break;
		}
	}

	if (result != RUNNING) {
		resetState(entity);
	}

	return result;
}

void Sequence::resetState(AI& entity) {
	setSelectorState(entity, NOTHING_SELECTED);
	TreeNode::resetState(entity);
}

TreeNodePtr Sequence::Factory::create(const TreeNodeFactoryContext *ctx) const {
	return TreeNodePtr(new Sequence(ctx->name));
}

Sequence::Factory Sequence::FACTORY;

}
