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
class NodeTreeWidget: public QGraphicsView {
Q_OBJECT
private:
	AIDebugger& _debugger;
	QGraphicsScene* _scene;

	void buildTreeItems(int maxChild, int child, const AIStateNode& node, NodeTreeItem* parent);
	int getMaxChild(const AIStateNode& node, int maxChild) const;
public:
	NodeTreeWidget(AIDebugger& debugger);
	virtual ~NodeTreeWidget();

	void updateTreeWidget();
};

}
}
