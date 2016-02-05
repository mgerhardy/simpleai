#include "Zone.h"
#include "ICharacter.h"

namespace ai {

bool Zone::addAI(const AIPtr& ai) {
	if (ai == nullptr)
		return false;
	const CharacterId& id = ai->getCharacter()->getId();
	{
		ScopedReadLock scopedLock(_lock);
		if (_ais.find(id) != _ais.end())
			return false;
	}
	ScopedWriteLock scopedLock(_lock);
	_ais.insert(std::make_pair(id, ai));
	ai->setZone(this);
	return true;
}

bool Zone::removeAI(const AIPtr& ai) {
	if (!ai)
		return false;
	const CharacterId& id = ai->getCharacter()->getId();
	ScopedWriteLock scopedLock(_lock);
	AIMapIter i = _ais.find(id);
	if (i == _ais.end())
		return false;
	i->second->setZone(nullptr);
	_groupManager.removeFromAllGroups(i->second);
	_ais.erase(i);
	return true;
}

bool Zone::destroyAI(const CharacterId& id) {
	ScopedWriteLock scopedLock(_lock);
	AIMapIter i = _ais.find(id);
	if (i == _ais.end())
		return false;
	_ais.erase(i);
	return true;
}

bool Zone::scheduleAdd(const AIPtr& ai) {
	if (!ai)
		return false;
	ScopedWriteLock scopedLock(_scheduleLock);
	_scheduledAdd.push_back(ai);
	return true;
}

bool Zone::scheduleDestroy(const CharacterId& id) {
	ScopedWriteLock scopedLock(_scheduleLock);
	_scheduledDestroy.push_back(id);
	return true;
}

bool Zone::scheduleRemove(const AIPtr& ai) {
	if (!ai)
		return false;
	ScopedWriteLock scopedLock(_scheduleLock);
	_scheduledRemove.push_back(ai);
	return true;
}

void Zone::update(int64_t dt) {
	{
		ScopedWriteLock scopedLock(_scheduleLock);
		for (const AIPtr& ai : _scheduledAdd) {
			addAI(ai);
		}
		_scheduledAdd.clear();
		for (const AIPtr& ai : _scheduledRemove) {
			removeAI(ai);
		}
		_scheduledRemove.clear();
		for (auto id : _scheduledDestroy) {
			destroyAI(id);
		}
		_scheduledDestroy.clear();
	}

	auto func = [&] (const AIPtr& ai) {
		if (ai->isPause())
			return;
		ai->update(dt, _debug);
		ai->getBehaviour()->execute(ai, dt);
	};
	visit(func);
	_groupManager.update(dt);
}

}
