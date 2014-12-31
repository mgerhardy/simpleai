#pragma once

#include <filter/IFilter.h>

namespace ai {

/**
 * @brief This filter just clears the selection
 */
class SelectEmpty: public IFilter {
public:
	FILTER_CLASS_SINGLETON(SelectEmpty)

	void filter (const AI& entity) override;
};

inline void SelectEmpty::filter (const AI& entity) {
	getFilteredEntities(entity).clear();
}

}
