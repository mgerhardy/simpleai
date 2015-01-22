#include "Zone.h"
#include "ICharacter.h"

namespace ai {

bool Zone::addAI(AI* ai) {
	if (ai == nullptr)
		return false;
	const CharacterId& id = ai->getCharacter().getId();
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

bool Zone::removeAI(const AI* ai) {
	if (ai == nullptr)
		return false;
	const CharacterId& id = ai->getCharacter().getId();
	ScopedWriteLock scopedLock(_lock);
	AIMapIter i = _ais.find(id);
	if (i == _ais.end())
		return false;
	i->second->setZone(nullptr);
	_groupManager.removeFromAllGroups(&i->second->getCharacter());
	_ais.erase(i);
	return true;
}

void Zone::update(long dt) {
	auto func = [&] (AI& ai) {
		ai.getCharacter().update(dt, _debug);
	};
	visit(func);
	_groupManager.update(dt);
}

}
