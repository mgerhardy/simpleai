#include "NodeTreeWidget.h"
#include <AI.h>

namespace ai {
namespace debug {

NodeTreeWidget::NodeTreeWidget(AIDebugger& debugger) :
		QFrame(), _debugger(debugger) {
}

NodeTreeWidget::~NodeTreeWidget() {
}

}
}
