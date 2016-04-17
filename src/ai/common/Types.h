#pragma once

#include <string>
#include <unordered_map>
#include <cassert>
#include <iostream>

#if !(__GNUC__ || __GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#ifndef ai_assert
#if (__clang_analyzer__)
#define ai_assert assert
#else
#define ai_assert(condition, format, ...) \
	if ( !(condition) ) { \
		static char buf[1024]; \
		std::snprintf(buf, sizeof(buf), format, ##__VA_ARGS__); \
		std::cerr << __FILE__ << ":" << __LINE__ << "@" << __PRETTY_FUNCTION__ << ": " << buf << std::endl; \
		assert(condition); \
	}
#endif
#endif

template<class T, class S>
inline T ai_assert_cast(const S object) {
#ifdef __cpp_rtti
	ai_assert(dynamic_cast<T>(object) == static_cast<T>(object), "Types don't match");
#endif
	return static_cast<T>(object);
}

#define AI_STRINGIFY_INTERNAL(x) #x
#define AI_STRINGIFY(x) AI_STRINGIFY_INTERNAL(x)

#ifndef AI_EXCEPTIONS
#define AI_EXCEPTIONS 0
#endif

#ifdef _WIN32
#	ifdef SIMPLEAI_EXPORT
#		define SIMPLEAI_LIB __declspec(dllexport)
#	elif defined(SIMPLEAI_IMPORT)
#		define SIMPLEAI_LIB __declspec(dllimport)
#	else
#		define SIMPLEAI_LIB
#	endif
#else
#	define SIMPLEAI_LIB
#endif

namespace ai {

/**
 * @brief Defines the type of the id to identify an ai controlled entity.
 *
 * @note @c -1 is reserved. You should use ids >= 0
 * @sa NOTHING_SELECTED
 */
typedef int CharacterId;

typedef std::unordered_map<std::string, std::string> CharacterAttributes;

}
