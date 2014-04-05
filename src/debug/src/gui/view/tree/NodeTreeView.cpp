#include "NodeTreeView.h"
#include <AI.h>

namespace {
const int horizontalSpacing = 40;
const int verticalSpacing = 10;
const int nodeHeight = 60;
}

namespace ai {
namespace debug {

NodeTreeView::NodeTreeView(AIDebugger& debugger) :
		QGraphicsView(), _debugger(debugger), _scene(this) {
	_scene.setItemIndexMethod(QGraphicsScene::NoIndex);
	// because the connection lines are not included in the bounding box...
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	setScene(&_scene);
}

NodeTreeView::~NodeTreeView() {
}

void NodeTreeView::updateTreeWidget() {
	_scene.clear();
	const ai::CharacterId& id = _debugger.getSelected();
	if (id == -1) {
		return;
	}
	const AIStateNode& node = _debugger.getNode();
	NodeTreeItem* item = buildTreeItems(node, nullptr);
	item->init();
}

NodeTreeItem* NodeTreeView::buildTreeItems(const AIStateNode& node, NodeTreeItem* parent) {
	NodeTreeItem* thisNode = new NodeTreeItem(node, parent, nodeHeight, horizontalSpacing, verticalSpacing);
	_scene.addItem(thisNode);
	const std::vector<AIStateNode>& children = node.getChildren();
	for (std::vector<AIStateNode>::const_iterator i = children.begin(); i != children.end(); ++i) {
		NodeTreeItem* childNode = buildTreeItems(*i, thisNode);
		thisNode->addChildren(childNode);
	}
	return thisNode;
}

}
}
