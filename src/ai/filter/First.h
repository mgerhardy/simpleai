#pragma once

#include "filter/IFilter.h"

namespace ai {

/**
 * @brief This filter will just preserve the first entry of other filters
 */
class First: public IFilter {
public:
	FILTER_ACTION_CLASS(First)
	FILTER_ACTION_FACTORY(First)

	void filter (const AIPtr& entity) override;
};

inline void First::filter (const AIPtr& entity) {
	FilteredEntities& filtered = getFilteredEntities(entity);
	const FilteredEntities copy = filtered;
	filtered.clear();
	_filters.front()->filter(entity);
	const auto& value = getFilteredEntities(entity).front();
	filtered.clear();
	for (auto& e : copy) {
		filtered.push_back(e);
	}
	filtered.push_back(value);
}

}
