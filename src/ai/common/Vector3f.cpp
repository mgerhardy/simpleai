#include "Vector3f.h"
#include "String.h"
#include <limits>

namespace ai {

Vector3f Vector3f::ZERO(0.0f, 0.0f, 0.0f);
Vector3f Vector3f::INFINITE(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());

Vector3f Vector3f::parse(const std::string& in) {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	if (::sscanf(in.c_str(), "%f:%f:%f", &x, &y, &z) != 3) {
		return INFINITE;
	}

	return Vector3f(x, y, z);
}

}
