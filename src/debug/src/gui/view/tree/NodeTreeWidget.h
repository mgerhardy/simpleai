#pragma once

#include <QtGui>

#include "AIDebugger.h"
#include <AI.h>

namespace ai {
namespace debug {

class NodeTreeWidget: public QFrame {
Q_OBJECT
private:
	AIDebugger& _debugger;
public:
	NodeTreeWidget(AIDebugger& debugger);
	virtual ~NodeTreeWidget();
};

}
}
