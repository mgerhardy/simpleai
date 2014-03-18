#include <conditions/HasEnemies.h>
#include <AI.h>

namespace ai {

bool HasEnemies::evaluate(AI& entity) {
	return entity.getAggroMgr().getHighestEntry();
}

CONDITION_FACTORY_IMPL(HasEnemies)

}
