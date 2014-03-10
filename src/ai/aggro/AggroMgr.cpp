#include <AI.h>
#include <aggro/AggroMgr.h>
#include <algorithm>

namespace ai {

AggroMgr::AggroMgr() :
		_lastUpdateTime(0L) {
}

AggroMgr::~AggroMgr() {
}

void AggroMgr::cleanupList() {
	std::size_t remove = 0u;
	for (Entries::reverse_iterator i = _entries.rbegin(); i != _entries.rend(); ++i) {
		const float aggroValue = (*i)->getAggro();
		if (aggroValue > 0.0f)
			break;

		++remove;
	}

	if (remove == 0u)
		return;

	const std::size_t size = _entries.size();
	if (size == remove)
		_entries.clear();
	else
		_entries.resize(size - remove);
}

void AggroMgr::update(long currentMillis) {
	const std::size_t size = _entries.size();
	for (std::size_t i = 0; i < size; ++i)
		_entries[i]->reduceByTime(currentMillis - _lastUpdateTime);

	cleanupList();
	_lastUpdateTime = currentMillis;
}

class CharacterIdPredicate {
private:
	const CharacterId& _id;
public:
	CharacterIdPredicate(const CharacterId& id) :
			_id(id) {
	}

	bool operator()(const EntryPtr &n1) {
		return n1->getEntity().getCharacter().getId() == _id;
	}
};

void AggroMgr::addAggro(AI& entity, float amount) {
	const CharacterIdPredicate p(entity.getCharacter().getId());
	Entries::const_iterator i = std::find_if(_entries.begin(), _entries.end(), p);
	if (i == _entries.end()) {
		const EntryPtr newEntry(new Entry(entity));
		newEntry->addAggro(amount);
		_entries.push_back(newEntry);
	} else {
		(*i)->addAggro(amount);
	}
	std::sort(_entries.begin(), _entries.end());
}

}
