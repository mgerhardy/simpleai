#pragma once

#include "Vector3f.h"
#include <cmath>

namespace ai {
const float M_2PI = 2.0f * static_cast<float>(M_PI);

inline float toRadians (float degree) {
	return degree * static_cast<float>(M_PI) / 180.0f;
}

inline float toDegrees (float radians) {
	return radians * 180.0f / static_cast<float>(M_PI);
}

template<typename T>
inline T clamp(T a, T low, T high) {
	return std::max(low, std::min(a, high));
}

}
