#pragma once

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

}
