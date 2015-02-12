#include <filter/SelectZone.h>
#include <zone/Zone.h>

namespace ai {

void SelectZone::filter (const AI& entity) {
	FilteredEntities& entities = getFilteredEntities(entity);
	auto func = [&] (const AI& ai) {
		entities.push_back(ai.getCharacter().getId());
		return true;
	};
	entity.getZone().visit(func);
}

FILTER_FACTORY_IMPL(SelectZone)

}
