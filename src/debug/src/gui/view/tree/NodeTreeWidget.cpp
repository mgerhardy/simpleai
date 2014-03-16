#include "NodeTreeWidget.h"
#include <AI.h>

namespace ai {
namespace debug {

NodeTreeWidget::NodeTreeWidget(AIDebugger& debugger) :
		QGraphicsView(), _debugger(debugger) {
	_scene = new QGraphicsScene(this);
	setScene(_scene);
}

NodeTreeWidget::~NodeTreeWidget() {
	delete _scene;
}

void NodeTreeWidget::updateTreeWidget() {
	const ai::CharacterId id = _debugger.getSelected();
	if (id == -1) {
		_scene->clear();
		return;
	}
	const AIStateNode& node = _debugger.getNode();
	buildTreeItems(node, nullptr);
}

void NodeTreeWidget::buildTreeItems(const AIStateNode& node, NodeTreeItem* parent) {
	NodeTreeItem* thisNode = new NodeTreeItem(node, parent);
	_scene->addItem(thisNode);
	const std::vector<AIStateNode>& children = node.getChildren();
	for (std::vector<AIStateNode>::const_iterator i = children.begin(); i != children.end(); ++i) {
		buildTreeItems(*i, thisNode);
	}
}

}
}
