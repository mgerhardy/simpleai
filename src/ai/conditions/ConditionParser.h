#pragma once

#include <conditions/ICondition.h>
#include <common/IParser.h>

namespace ai {

class IAIFactory;

class ConditionParser : public IParser {
private:
	const IAIFactory& _aiFactory;
	std::string _conditionString;

	bool fillInnerConditions(ConditionFactoryContext& ctx, const std::string inner);

public:
	ConditionParser(const IAIFactory& aiFactory, const std::string& conditionString);
	virtual ~ConditionParser();

	ConditionPtr getCondition();
};

}
