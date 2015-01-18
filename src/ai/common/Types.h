#pragma once

#include <string>
#include <unordered_map>
#include <cassert>

#ifndef ai_assert
#define ai_assert(cond, msg) assert(cond)
#endif

#ifndef AI_EXCEPTIONS
#define AI_EXCEPTIONS 0
#endif

namespace ai {

/**
 * @brief Defines the type of the id to identify an ai controlled entity.
 *
 * @note @c -1 is reserved. You should use ids >= 0
 */
typedef int CharacterId;

typedef std::unordered_map<std::string, std::string> CharacterAttributes;

}
