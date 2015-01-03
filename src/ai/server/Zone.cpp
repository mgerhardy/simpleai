#include "Zone.h"
#include "ICharacter.h"

namespace ai {

bool Zone::addAI(AI* ai) {
	if (ai == nullptr)
		return false;
	const CharacterId& id = ai->getCharacter().getId();
	SCOPEDLOCK(_mutex);
	if (_ais.find(id) != _ais.end())
		return false;
	_ais.insert(std::make_pair(id, ai));
	return true;
}

bool Zone::removeAI(const AI* ai) {
	if (ai == nullptr)
		return false;
	const CharacterId& id = ai->getCharacter().getId();
	SCOPEDLOCK(_mutex);
	AIMapIter i = _ais.find(id);
	if (i == _ais.end())
		return false;
	_ais.erase(i);
	return true;
}

void Zone::update(long dt) {
	auto func = [&] (AI& ai) {
		ai.getCharacter().update(dt, _debug);
	};
	visit(func);
}

}
