#include <conditions/And.h>

namespace ai {

ConditionPtr And::Factory::create(const ConditionFactoryContext *ctx) const {
	if (ctx->conditions.size() < 2)
		return ConditionPtr();
	return ConditionPtr(new And(ctx->conditions));
}

void And::getConditionNameWithValue(std::stringstream& s, AI& entity) {
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

bool And::evaluate(AI& entity) {
	for (Conditions::iterator i = _conditions.begin(); i != _conditions.end(); ++i) {
		if (!(*i)->evaluate(entity))
			return false;
	}

	return true;
}

std::ostream& And::print(std::ostream& stream, int level) const {
	ICondition::print(stream, level);
	stream << "(";
	for (Conditions::const_iterator i = _conditions.begin(); i != _conditions.end();) {
		(*i)->print(stream, level);
		++i;
		if (i != _conditions.end())
			stream << ",";
	}
	stream << ")";
	return stream;
}

And::Factory And::FACTORY;

}
