#include "ITimedNode.h"

#include <stdlib.h>

namespace ai {

#define NOTSTARTED -1

ITimedNode::ITimedNode(const std::string& name, const std::string& parameters, const ConditionPtr& condition) :
		TreeNode(name, parameters, condition), _timerMillis(-1) {
	if (!parameters.empty())
		_millis = ::atol(parameters.c_str());
	else
		_millis = 1000L;
}

ITimedNode::~ITimedNode() {
}

TreeNodeStatus ITimedNode::execute(AI& entity, long currentMillis) {
	if (_timerMillis == NOTSTARTED) {
		_timerMillis = _millis;
	} else if (_timerMillis - currentMillis > 0) {
		_timerMillis -= currentMillis;
	} else {
		_timerMillis = NOTSTARTED;
		return FINISHED;
	}

	return executeTimed(entity, currentMillis);
}

}
