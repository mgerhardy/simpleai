#include "AINodeStaticResolver.h"
#include <SimpleAI.h>
#include <QObject>

namespace ai {
namespace debug {

namespace {
const AIStateNodeStatic UNKNOWN(-1, "unknown", "unknown");
}

AINodeStaticResolver::AINodeStaticResolver() {
}

const AIStateNodeStatic& AINodeStaticResolver::get(int32_t id) const {
	return UNKNOWN;
}

}
}
