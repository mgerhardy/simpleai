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
	FilteredEntities::value_type value = filtered.front();
	filtered.clear();
	filtered.push_back(value);
}

}
