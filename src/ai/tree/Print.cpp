#include "Print.h"

namespace ai {

void Print::tree(AI& entity) {
	const TreeNodePtr behaviour = entity.getBehaviour();
	std::cout << behaviour.get() << std::endl;
}

void Print::lastcondition(AI& /*entity*/) {
	// TODO:
}

void Print::lastnode(AI& /*entity*/) {
	// TODO:
}

void Print::handleCommand(AI& entity) {
	if (_parameters == "::tree") {
		tree(entity);
	} else if (_parameters == "::lastcondition") {
		lastcondition(entity);
	} else if (_parameters == "::lastnode") {
		lastnode(entity);
	} else {
		std::cout << "Unknown command: " << _parameters << std::endl;
	}
}

TreeNodeStatus Print::doAction(AI& entity, long /*deltaMillis*/) {
	if (ai::Str::startsWith(_parameters, "::")) {
		handleCommand(entity);
		return FINISHED;
	}
	std::cout << _parameters << std::endl;
	return FINISHED;
}


NODE_FACTORY_IMPL(Print)

}
