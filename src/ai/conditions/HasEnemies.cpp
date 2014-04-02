#include <conditions/HasEnemies.h>
#include <AI.h>

namespace ai {

HasEnemies::HasEnemies(const std::string& parameters) :
		ICondition("HasEnemies", parameters) {
	if (_parameters.empty())
		_enemyCount = -1;
	else
		_enemyCount = Str::toInt(_parameters);
}

bool HasEnemies::evaluate(AI& entity) {
	AggroMgr& mgr = entity.getAggroMgr();
	if (_enemyCount == -1)
		return mgr.getHighestEntry();
	const int size = mgr.getEntries().size();
	return size >= _enemyCount;
}

CONDITION_FACTORY_IMPL(HasEnemies)

}
