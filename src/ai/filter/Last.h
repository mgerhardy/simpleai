#pragma once

#include "filter/IFilter.h"

namespace ai {

/**
 * @brief This filter will just preserve the last entry of other filters
 */
class Last: public IFilter {
public:
	FILTER_ACTION_CLASS(Last)
	FILTER_ACTION_FACTORY(Last)

	void filter (const AIPtr& entity) override;
};

inline void Last::filter (const AIPtr& entity) {
	FilteredEntities& filtered = getFilteredEntities(entity);
	FilteredEntities::value_type value = filtered.back();
	filtered.clear();
	filtered.push_back(value);
}

}
