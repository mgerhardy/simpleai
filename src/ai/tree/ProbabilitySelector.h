#pragma once

#include "tree/Selector.h"
#include <vector>

namespace ai {

/**
 * @brief This node executes one of the attached children randomly based on the given weights.
 *
 * http://aigamedev.com/open/article/selector/
 */
class ProbabilitySelector: public Selector {
protected:
	std::vector<float> _weights;
	float _weightSum;
public:
	ProbabilitySelector(const std::string& name, const std::string& parameters, const ConditionPtr& condition);

	virtual ~ProbabilitySelector() {
	}

	NODE_FACTORY

	TreeNodeStatus execute(AI& entity, long deltaMillis) override;
};

}
