#pragma once

#include "ICondition.h"
#include "common/String.h"
#include "group/GroupMgr.h"

namespace ai {

/**
 * @brief Evaluates to true if you are the first member in a particular group
 *
 * The parameter that is expected is the group id
 */
class IsGroupLeader: public ICondition {
private:
	GroupId _groupId;

	IsGroupLeader(const std::string& parameters) :
		ICondition("IsGroupLeader", parameters) {
		_groupId = std::stoi(_parameters);
	}
public:
	virtual ~IsGroupLeader() {
	}
	CONDITION_FACTORY

	bool evaluate(AI& entity) override;
};

}
