#pragma once

#include <filter/IFilter.h>

namespace ai {

/**
 * @brief This filter will pick the entity with the highest aggro value
 */
class SelectHighestAggro: public IFilter {
public:
	FILTER_CLASS_SINGLETON(SelectHighestAggro)

	void filter (const AI& entity) override;
};

inline void SelectHighestAggro::filter (const AI& entity) {
	const EntryPtr& entry = entity.getAggroMgr().getHighestEntry();
	if (!entry)
		return;

	const CharacterId id = entry->getCharacterId();
	getFilteredEntities(entity).push_back(id);
}

}
