#pragma once

#include <chrono>
#include <random>
#include <stdlib.h>

namespace ai {

inline void randomSeed (unsigned int seed) {
	srand(seed);
}

inline float randomf (float max = 1.0f) {
	return max * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
}

inline int random (int min, int max) {
	return static_cast<int>(randomf() * static_cast<float>((max - min + 1) + min));
}

inline float randomBinomial (float max = 1.0f) {
	return randomf(max) - randomf(max);
}

template<typename I>
inline I randomElement(I begin, I end) {
	const int n = static_cast<int>(std::distance(begin, end));
	auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::uniform_int_distribution<> dis(0, n);
	std::default_random_engine generator(seed);
	std::advance(begin, dis(generator));
	return begin;
}

}
