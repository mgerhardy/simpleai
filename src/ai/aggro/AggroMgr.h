#pragma once

#include <vector>
#include <aggro/Entry.h>

namespace ai {

class AI;

class AggroMgr {
public:
	typedef std::vector<EntryPtr> Entries;
protected:
	Entries _entries;

	bool _dirty;

	/**
	 * @brief Remove the entries from the list that have no aggro left.
	 * This list is ordered, so we will only remove the first X elements.
	 */
	void cleanupList();

	inline void sort();
public:
	AggroMgr(int expectedEntrySize = 0);
	virtual ~AggroMgr();

	/**
	 * @brief this will update the aggro list according to the reduction type of an entry.
	 * @param[in] deltaMillis The current milliseconds to use to update the aggro value of the entries.
	 */
	void update(long deltaMillis);

	/**
	 * @brief will increase the aggro
	 * @param[in,out] entity The entity to increase the aggro against
	 * @param[in] amount The amount to increase the aggro for
	 */
	Entry* addAggro(AI& entity, float amount);

	/**
	 * @return All the aggro entries
	 */
	const Entries& getEntries() const {
		return _entries;
	}

	/**
	 * @brief Get the entry with the highest aggro value.
	 */
	EntryPtr getHighestEntry();
};

}
