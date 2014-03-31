#pragma once

#include "ICondition.h"
#include "common/String.h"
#include "group/GroupMgr.h"

namespace ai {

class IsCloseToGroup: public ICondition {
private:
	GroupId _groupId;
	float _distance;

	IsCloseToGroup(const std::string& parameters) :
		ICondition("IsCloseToGroup", parameters) {
		std::vector<std::string> tokens;
		Str::splitString(_parameters, tokens, ",");
		if (tokens.size() != 2) {
			_groupId = -1;
			_distance = -1.0f;
		} else {
			_groupId = Str::toInt(tokens[0]);
			_distance = Str::toFloat(tokens[1]);
		}
	}
public:
	virtual ~IsCloseToGroup() {
	}
	CONDITION_FACTORY

	bool evaluate(AI& entity) override;
};

}
