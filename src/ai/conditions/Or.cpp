#include <conditions/Or.h>

namespace ai {

ConditionPtr Or::Factory::create(const ConditionFactoryContext *ctx) const {
	if (ctx->conditions.size() < 2)
		return ConditionPtr();
	return ConditionPtr(new Or(ctx->conditions));
}

void Or::getConditionNameWithValue(std::stringstream& s, AI& entity) {
	bool first = true;
	for (Conditions::iterator i = _conditions.begin(); i != _conditions.end(); ++i) {
		if (!first)
			s << "|";
		s << (*i)->getName() << "[" << ((*i)->evaluate(entity) ? "1" : "0") << "]";
		first = false;
	}
}

bool Or::evaluate(AI& entity) {
	for (Conditions::iterator i = _conditions.begin(); i != _conditions.end(); ++i) {
		if ((*i)->evaluate(entity))
			return true;
	}

	return false;
}

Or::Factory Or::FACTORY;

}
