#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>

#include "NodeTreeItem.h"
#include "AIDebugger.h"
#include <AI.h>

namespace ai {
namespace debug {

/**
 * @brief Shows the behaviour tree for the current selected entity
 */
class NodeTreeView: public QGraphicsView {
Q_OBJECT
private:
	AIDebugger& _debugger;
	QGraphicsScene _scene;

	NodeTreeItem* buildTreeItems(const AIStateNode& node, NodeTreeItem* parent);
public:
	NodeTreeView(AIDebugger& debugger);
	virtual ~NodeTreeView();

	void updateTreeWidget();
};

}
}
