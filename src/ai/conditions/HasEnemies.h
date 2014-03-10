#pragma once

#include <conditions/ICondition.h>

namespace ai {

/**
 * @brief This condition checks whether there is a aggro entry available.
 *
 * @sa AggroMgr
 */
class HasEnemies: public ICondition {
public:
	CONDITION_CLASS_SINGLETON(HasEnemies);

	bool evaluate(AI& entity) override;
};

}
