#include "Vector3f.h"
#include <limits>

namespace ai {

Vector3f Vector3f::ZERO(0.0f, 0.0f, 0.0f);
Vector3f Vector3f::INVALID(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());

}
