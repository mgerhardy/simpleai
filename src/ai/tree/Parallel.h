#pragma once

#include <tree/Selector.h>

namespace ai {

/**
 * @brief Executes all the connected children in the order they were added (no matter what
 * the TreeNodeStatus of the previous child was).
 *
 * http://aigamedev.com/open/article/parallel/
 */
class Parallel: public Selector {
public:
	SELECTOR_CLASS(Parallel)

	void getChildrenState(const AI& entity, std::vector<bool>& active) const override;
	/**
	 * @brief If one of the children was executed, and is still running, the ::TreeNodeStatus::RUNNING
	 * is returned, otherwise ::TreeNodeStatus::FINISHED is returned.
	 */
	TreeNodeStatus execute(AI& entity, long deltaMillis) override;
};

}
