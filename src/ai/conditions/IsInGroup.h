#pragma once

#include "ICondition.h"
#include "common/String.h"
#include "group/GroupMgr.h"

namespace ai {

class IsInGroup: public ICondition {
private:
	GroupId _groupId;

	IsInGroup(const std::string& parameters) :
		ICondition("IsInGroup", parameters) {
		if (_parameters.empty())
			_groupId = -1;
		else
			_groupId = Str::toInt(_parameters);
	}
public:
	virtual ~IsInGroup() {
	}
	CONDITION_FACTORY

	bool evaluate(AI& entity) override;
};

}
