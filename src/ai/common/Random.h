#pragma once

#include <stdlib.h>

namespace ai {

inline float randomReal (float max = 1.0f) {
	return max * (float(rand()) / RAND_MAX);
}

inline float randomBinomial (float max = 1.0f) {
	return randomReal(max) - randomReal(max);
}

}
