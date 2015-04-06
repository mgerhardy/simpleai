#include "Print.h"
#include "ICharacter.h"

namespace ai {

void Print::tree(AI& entity) const {
	const TreeNodePtr behaviour = entity.getBehaviour();
	std::cout << behaviour.get() << std::endl;
}

void Print::lastcondition(AI& /*entity*/) const {
	// TODO:
}

void Print::lastnode(AI& /*entity*/) const {
	// TODO:
}

void Print::attributes(AI& entity) const {
	const CharacterAttributes& attribs = entity.getCharacter().getAttributes();
	for (auto i = attribs.begin(); i != attribs.end(); ++i) {
		std::cout << i->first << ": " << i->second << std::endl;
	}
}

TreeNodeStatus Print::handleCommand(AI& entity) const {
	if (_parameters == "::tree") {
		tree(entity);
	} else if (_parameters == "::lastcondition") {
		lastcondition(entity);
	} else if (_parameters == "::lastnode") {
		lastnode(entity);
	} else if (_parameters == "::attributes") {
		attributes(entity);
	} else {
		std::cout << "Unknown command: " << _parameters << std::endl;
		return FAILED;
	}
	return FINISHED;
}

TreeNodeStatus Print::doAction(AI& entity, long /*deltaMillis*/) {
	if (ai::Str::startsWith(_parameters, "::")) {
		return handleCommand(entity);
	}
	std::cout << _parameters << std::endl;
	return FINISHED;
}

NODE_FACTORY_IMPL(Print)

}
