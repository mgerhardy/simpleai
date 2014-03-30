#pragma once

#include "Vector3f.h"
#include <cmath>

namespace ai {
const double M_2PI = 6.28318530717958647692;

inline float toRadians (float degree) {
	return degree * M_PI / 180.0f;
}

inline float toDegrees (float radians) {
	return radians * 180.0f / M_PI;
}

}
