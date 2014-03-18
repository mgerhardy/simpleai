#include <conditions/ConditionParser.h>
#include <AIRegistry.h>
#include <iostream>

namespace ai {

ConditionParser::ConditionParser(const IAIFactory& aiFactory, const std::string& conditionString) :
		IParser(), _aiFactory(aiFactory) {
	_conditionString = eraseAllSpaces(conditionString);
}

ConditionParser::~ConditionParser() {
}

bool ConditionParser::fillInnerConditions(ConditionFactoryContext& ctx, const std::string conditionStr) {
	std::vector<std::string> conditions;
	splitString(conditionStr, conditions, ",");
	if (conditions.size() > 1) {
		for (std::vector<std::string>::const_iterator i = conditions.begin(); i != conditions.end(); ++i) {
			if (!fillInnerConditions(ctx, *i))
				return false;
		}
	} else {
		const std::string& parameters = getBetween(conditionStr, "{", "}");
		std::size_t n = conditionStr.find("{");
		if (n == std::string::npos)
			n = conditionStr.find("(");
		std::string name;
		if (n != std::string::npos) {
			name = conditionStr.substr(0, n);
		} else {
			name = conditionStr;
		}
		ConditionFactoryContext ctxInner(parameters);
		if (n != std::string::npos) {
			const std::size_t r = conditionStr.rfind(")");
			if (r == std::string::npos) {
				setError("syntax error, missing closing brace");
				return false;
			}
			const std::string& inner = conditionStr.substr(n + 1, r - n - 1);
			if (!fillInnerConditions(ctxInner, inner))
				return false;
		}
		const ConditionPtr& c = _aiFactory.createCondition(name, ctxInner);
		if (!c) {
			setError("could not create inner condition for " + name);
			return false;
		}
		ctx.conditions.push_back(c);
	}
	return true;
}

ConditionPtr ConditionParser::getCondition() {
	const std::string& parameters = getBetween(_conditionString, "{", "}");
	std::size_t n = _conditionString.find("{");
	if (n == std::string::npos)
		n = _conditionString.find("(");
	std::string name;
	if (n != std::string::npos) {
		name = _conditionString.substr(0, n);
	} else {
		name = _conditionString;
	}
	ConditionFactoryContext ctx(parameters);
	if (n != std::string::npos) {
		const std::size_t r = _conditionString.rfind(")");
		if (r == std::string::npos) {
			setError("syntax error, missing closing brace");
			return ConditionPtr();
		}
		const std::string inner = _conditionString.substr(n + 1, r - n - 1);
		if (!fillInnerConditions(ctx, inner)) {
			return ConditionPtr();
		}
	}
	const ConditionPtr& c = _aiFactory.createCondition(name, ctx);
	if (!c) {
		setError("could not create condition for " + name);
		return ConditionPtr();
	}
	return c;
}

}
