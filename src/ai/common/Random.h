#pragma once

#include <stdlib.h>

namespace ai {

inline void randomSeed (unsigned int seed) {
	srand(seed);
}

inline float randomf (float max = 1.0f) {
	return max * (float(rand()) / RAND_MAX);
}

inline float randomBinomial (float max = 1.0f) {
	return randomf(max) - randomf(max);
}

}
