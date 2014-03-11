#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>

#include "NodeTreeItem.h"
#include "AIDebugger.h"
#include <AI.h>

namespace ai {
namespace debug {

class NodeTreeWidget: public QGraphicsView {
Q_OBJECT
private:
	AIDebugger& _debugger;
	QGraphicsScene* _scene;

	void buildTreeItems(const AIStateNode& node, NodeTreeItem* parent);
public:
	NodeTreeWidget(AIDebugger& debugger);
	virtual ~NodeTreeWidget();

	void updateTreeWidget();
};

}
}
