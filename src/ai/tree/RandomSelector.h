#pragma once

#include "tree/Selector.h"

namespace ai {

/**
 * @brief This node executes all the attached children in random order. This composite only
 * fails if all children failed, too.
 *
 * http://aigamedev.com/open/article/selector/
 */
class RandomSelector: public Selector {
public:
	SELECTOR_CLASS(RandomSelector)

	TreeNodeStatus execute(AI& entity, long deltaMillis) override;
};

}
