#pragma once

#include "tree/TreeNode.h"

namespace ai {

#define TIMERNODE_CLASS(NodeName) \
	NodeName(const std::string& name, const std::string& parameters, const ConditionPtr& condition) : \
		ITimedNode(name, parameters, condition) { \
	} \
	virtual ~NodeName() { \
	} \
	\
	NODE_FACTORY

class ITimedNode : public TreeNode {
protected:
	long _timerMillis;
	long _millis;
public:
	ITimedNode(const std::string& name, const std::string& parameters, const ConditionPtr& condition);
	virtual ~ITimedNode();

	TreeNodeStatus execute(AI& entity, long deltaMillis);

	virtual TreeNodeStatus executeTimed(AI& entity, long deltaMillis) = 0;
};

}
