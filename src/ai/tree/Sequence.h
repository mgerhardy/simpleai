#pragma once

#include <tree/Selector.h>

namespace ai {

/**
 * @brief The sequence continues to execute their children until one of the children
 * returned a state that is not equal to finished. On the next iteration the execution
 * is continued at the last running children or from the start again if no such
 * children exists.
 *
 * http://aigamedev.com/open/article/sequence/
 */
class Sequence: public Selector {
public:
	SELECTOR_CLASS(Sequence);

	TreeNodeStatus execute(AI& entity, long currentMillis) override;
	void resetState(AI& entity) override;
};

}
