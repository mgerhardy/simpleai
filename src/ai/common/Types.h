#pragma once

#include <string>
#include <map>

namespace ai {

/**
 * @brief Defines the type of the id to identify an ai controlled entity.
 *
 * @note @c -1 is reserved. You should use ids >= 0
 */
typedef int CharacterId;

typedef std::map<std::string, std::string> CharacterAttributes;

}
