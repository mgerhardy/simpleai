#pragma once

#include <conditions/ICondition.h>

namespace ai {

/**
 * @brief This condition just always evaluates to @c true
 */
class True: public ICondition {
public:
	CONDITION_CLASS_SINGLETON(True)

	bool evaluate(const AI& entity) override;

	std::ostream& print(std::ostream& stream, int level) const override;
};

inline bool True::evaluate(const AI& /* entity */) {
	return true;
}

}
