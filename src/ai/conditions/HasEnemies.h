#pragma once

#include <conditions/ICondition.h>

namespace ai {

/**
 * @brief This condition checks whether there is an aggro @c Entry in the @¢ AggroMgr available.
 */
class HasEnemies: public ICondition {
public:
	CONDITION_CLASS(HasEnemies)
	CONDITION_FACTORY

	bool evaluate(AI& entity) override;
};

}
