#pragma once

#include "ICondition.h"
#include "common/String.h"
#include "group/GroupMgr.h"

namespace ai {

class IsGroupLeader: public ICondition {
private:
	GroupId _groupId;

	IsGroupLeader(const std::string& parameters) :
		ICondition("IsGroupLeader", parameters) {
		_groupId = Str::toInt(_parameters);
	}
public:
	virtual ~IsGroupLeader() {
	}
	CONDITION_FACTORY

	bool evaluate(AI& entity) override;
};

}
