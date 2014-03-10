#include <conditions/Not.h>

namespace ai {

ConditionPtr Not::Factory::create(const ConditionFactoryContext *ctx) const {
	if (ctx->conditions.size() != 1)
		return ConditionPtr();
	return ConditionPtr(new Not(ctx->conditions.front()));
}

Not::Factory Not::FACTORY;

}
