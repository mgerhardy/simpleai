#pragma once

#include "AI.h"
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
 *   - ::lastcondition
 *     Will print the state of the last condition
 *   - ::lastnode
 *     Will print the state of the last node
 *   - ::attributes
 *     Will print the entity attributes
 *
 * Every other parameter will just be directly forwarded to be printed.
 */
class Print: public ITask {
private:
	void tree(AI& entity) const;
	void lastcondition(AI& entity) const;
	void lastnode(AI& entity) const;
	void attributes(AI& entity) const;

	TreeNodeStatus handleCommand(AI& entity) const;

public:
	TASK_CLASS(Print)
	NODE_FACTORY

	TreeNodeStatus doAction(AI& entity, long deltaMillis) override;
};

}
