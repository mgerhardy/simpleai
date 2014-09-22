#pragma once

#include "IGraphicsView.h"
#include <QGraphicsScene>

#include "NodeTreeItem.h"
#include "AIDebugger.h"
#include <AI.h>

namespace ai {
namespace debug {

/**
 * @brief Shows the behaviour tree for the current selected entity
 */
class NodeTreeView: public IGraphicsView {
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
