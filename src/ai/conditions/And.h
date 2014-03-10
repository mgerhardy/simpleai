#pragma once

#include <conditions/ICondition.h>

namespace ai {

/**
 * @brief This condition will logically and all contained conditions
 */
class And: public ICondition {
protected:
	Conditions _conditions;

	void getConditionNameWithValue(std::stringstream& s, AI& entity);

public:
	And(const Conditions& conditions) :
			ICondition("And"), _conditions(conditions) {
	}
	virtual ~And() {
	}

	CONDITION_FACTORY

	bool evaluate(AI& entity) override;
};

}
