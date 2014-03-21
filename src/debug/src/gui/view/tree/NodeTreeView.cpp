#include "NodeTreeView.h"
#include <AI.h>

namespace ai {
namespace debug {

NodeTreeView::NodeTreeView(AIDebugger& debugger) :
		QGraphicsView(), _debugger(debugger) {
	_scene = new QGraphicsScene(this);
	setScene(_scene);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

NodeTreeView::~NodeTreeView() {
	delete _scene;
}

void NodeTreeView::updateTreeWidget() {
	_scene->clear();
	const ai::CharacterId& id = _debugger.getSelected();
	if (id == -1) {
		return;
	}
	const AIStateNode& node = _debugger.getNode();
	const int maxChild = getMaxChild(node, 1);
	buildTreeItems(maxChild, 0, node, nullptr);
}

int NodeTreeView::getMaxChild(const AIStateNode& node, int maxChild) const {
	const std::vector<AIStateNode>& children = node.getChildren();
	const size_t size = children.size();
	maxChild += size - 1;
	for (std::vector<AIStateNode>::const_iterator i = children.begin(); i != children.end(); ++i) {
		maxChild = std::max(maxChild, getMaxChild(*i, maxChild));
	}
	return maxChild;
}

void NodeTreeView::buildTreeItems(int maxChild, int child, const AIStateNode& node, NodeTreeItem* parent) {
	NodeTreeItem* thisNode = new NodeTreeItem(maxChild, child, node, parent);
	_scene->addItem(thisNode);
	const std::vector<AIStateNode>& children = node.getChildren();
	child = 0;
	for (std::vector<AIStateNode>::const_iterator i = children.begin(); i != children.end(); ++i, ++child) {
		buildTreeItems(maxChild, child, *i, thisNode);
	}
}

}
}
