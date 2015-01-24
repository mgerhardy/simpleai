#include "tree/ProbabilitySelector.h"
#include "AI.h"
#include "common/String.h"
#include "common/Random.h"

namespace ai {

ProbabilitySelector::ProbabilitySelector(const std::string& name, const std::string& parameters, const ConditionPtr& condition) :
		Selector(name, parameters, condition), _weightSum(0.0f) {
	std::vector<std::string> tokens;
	Str::splitString(parameters, tokens, ",");
	const int weightAmount = tokens.size();
	for (int i = 0; i < weightAmount; i++) {
		const float weight = ::atof(tokens[i].c_str());
		_weightSum += weight;
		_weights[i] = weight;
	}
}

TreeNodeStatus ProbabilitySelector::execute(AI& entity, long deltaMillis) {
	setSelectorState(entity, NOTHING_SELECTED);
	if (Selector::execute(entity, deltaMillis) == CANNOTEXECUTE)
		return CANNOTEXECUTE;

	int rndIndex = ai::random(0.0f, _weightSum);
	const int weightAmount = _weights.size();
	int i = 0;
	for (; i < weightAmount; i++) {
		if (rndIndex < _weights[i])
			break;
		rndIndex -= _weights[i];
	}

	const TreeNodePtr& child = _children[rndIndex];
	const TreeNodeStatus result = child->execute(entity, deltaMillis);
	if (result == RUNNING) {
		setSelectorState(entity, static_cast<int>(i));
	}
	child->resetState(entity);

	const int size = _children.size();
	for (int i = 0; i < size; ++i) {
		if (i == rndIndex)
			continue;
		_children[i]->resetState(entity);
	}
	return state(entity, result);
}

NODE_FACTORY_IMPL(ProbabilitySelector)

}
