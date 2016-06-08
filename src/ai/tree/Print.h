#pragma once

#include "AI.h"
#include "ICharacter.h"
#include "tree/ITask.h"
#include "common/String.h"
#include <iostream>

namespace ai {

/**
 * @brief The print task can print several states for an @code AI entity or just some
 * arbitrary string. Special commands are started with @code '::'.
 *
 * Available parameters:
 *   - ::tree
 *     Will print the current behaviour tree of the entity
 *   - ::attributes
 *     Will print the entity attributes
 *
 * Every other parameter will just be directly forwarded to be printed.
 */
class Print: public ITask {
private:
	void tree(const AIPtr& entity) const;
	void attributes(const AIPtr& entity) const;

	TreeNodeStatus handleCommand(const AIPtr& entity) const;

public:
	TASK_CLASS(Print)
	NODE_FACTORY(Print)

	TreeNodeStatus doAction(const AIPtr& entity, int64_t deltaMillis) override;
};

inline void Print::tree(const AIPtr& entity) const {
	const TreeNodePtr behaviour = entity->getBehaviour();
	std::cout << behaviour.get() << std::endl;
}

inline void Print::attributes(const AIPtr& entity) const {
	const CharacterAttributes& attribs = entity->getCharacter()->getAttributes();
	for (auto i = attribs.begin(); i != attribs.end(); ++i) {
		std::cout << i->first << ": " << i->second << std::endl;
	}
}

inline TreeNodeStatus Print::handleCommand(const AIPtr& entity) const {
	if (_parameters == "::tree") {
		tree(entity);
	} else if (_parameters == "::attributes") {
		attributes(entity);
	} else {
		std::cout << "Unknown command: " << _parameters << std::endl;
		return FAILED;
	}
	return FINISHED;
}

inline TreeNodeStatus Print::doAction(const AIPtr& entity, int64_t /*deltaMillis*/) {
	if (ai::Str::startsWith(_parameters, "::")) {
		return handleCommand(entity);
	}
	std::cout << _parameters << std::endl;
	return FINISHED;
}

}
