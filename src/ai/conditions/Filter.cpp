#include <conditions/Filter.h>
#include <AIRegistry.h>

namespace ai {

Filter::Filter (const Filters& filters) :
		ICondition("Filter", ""), _filters(filters) {
}

bool Filter::evaluate(const AI& entity) {
	entity._filteredEntities.clear();
	for (const FilterPtr filter : _filters) {
		filter->filter(entity);
	}
	return !entity._filteredEntities.empty();
}

ConditionPtr Filter::Factory::create(const ConditionFactoryContext *ctx) const {
	Filter* c = new Filter(ctx->filters);
	return ConditionPtr(c);
}

Filter::Factory Filter::FACTORY;

}
