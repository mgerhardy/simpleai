#pragma once

#include "filter/IFilter.h"
#include <random>
#include <algorithm>
#include <iterator>

namespace ai {

/**
 * @brief This filter will preserve only a few random entries
 */
class Random: public IFilter {
protected:
	Filters _filters;
	int _n;
	std::random_device _rd;
	std::mt19937 _g;
public:
	Random(const std::string& parameters, const Filters& filters) :
		IFilter("Random", parameters), _filters(filters), _g(_rd()) {
		ai_assert(filters.size() == 1, "Random must have one child");
		_n = std::stoi(parameters);
	}

	FILTER_ACTION_FACTORY(Random)

	void filter (const AIPtr& entity) override;
};

inline void Random::filter (const AIPtr& entity) {
	FilteredEntities& filtered = getFilteredEntities(entity);
	const FilteredEntities copy = filtered;
	filtered.clear();
	_filters.front()->filter(entity);
	FilteredEntities rndFiltered = getFilteredEntities(entity);
	std::shuffle(rndFiltered.begin(), rndFiltered.end(), _g);
	rndFiltered.resize(_n);
	for (auto& e : copy) {
		filtered.push_back(e);
	}
	for (auto& e : rndFiltered) {
		filtered.push_back(e);
	}
}

}
