#include <conditions/Or.h>

namespace ai {

ConditionPtr Or::Factory::create(const ConditionFactoryContext *ctx) const {
	if (ctx->conditions.size() < 2)
		return ConditionPtr();
	return ConditionPtr(new Or(ctx->conditions));
}

void Or::getConditionNameWithValue(std::stringstream& s, AI& entity) {
	bool first = true;
	s << "(";
	for (Conditions::iterator i = _conditions.begin(); i != _conditions.end(); ++i) {
		if (!first)
			s << ",";
		s << (*i)->getNameWithConditions(entity);
		first = false;
	}
	s << ")";
}

bool Or::evaluate(AI& entity) {
	for (Conditions::iterator i = _conditions.begin(); i != _conditions.end(); ++i) {
		if ((*i)->evaluate(entity))
			return true;
	}

	return false;
}

std::ostream& Or::print(std::ostream& stream, int level) const {
	ICondition::print(stream, level);
	stream << "(";
	for (Conditions::const_iterator i = _conditions.begin(); i != _conditions.end(); ++i) {
		(*i)->print(stream, level);
	}
	stream << ")";
	return stream;
}

Or::Factory Or::FACTORY;

}
