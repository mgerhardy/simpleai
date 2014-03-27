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
		QGraphicsView(), _debugger(debugger) {
	_scene = new QGraphicsScene(this);
	setScene(_scene);
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

// TODO: items might still overlap
void NodeTreeView::place(NodeTreeItem* node, NodeTreeItem* parent, int maxChild, int child) {
	// the size of the parent children - used to determine where 'node' should get rendered
	const size_t size = parent == nullptr ? 1 : parent->getSize();
	const int height = node->height();
	const qreal maxHeight = maxChild * height;
	const qreal half = maxHeight / 2.0;
	qreal y;
	if (size <= 1) {
		y = 0.0;
	} else {
		const qreal boxHeight = height + verticalSpacing;
		const qreal parentCenter = height / 2.0;
		const qreal startY = ((size * height) + (size - 1) * verticalSpacing) / -2.0;
		y = parentCenter + startY + child * boxHeight;
	}
	if (parent != nullptr) {
		node->setPos(parent->pos() + QPointF(parent->width() + horizontalSpacing, y));
	} else {
		y = half - height / 2.0;
		node->setPos(0.0, y);
	}
}

void NodeTreeView::buildTreeItems(int maxChild, int child, const AIStateNode& node, NodeTreeItem* parent) {
	NodeTreeItem* thisNode = new NodeTreeItem(node, parent, nodeHeight, horizontalSpacing, verticalSpacing);
	place(thisNode, parent, maxChild, child);
	_scene->addItem(thisNode);
	const std::vector<AIStateNode>& children = node.getChildren();
	child = 0;
	for (std::vector<AIStateNode>::const_iterator i = children.begin(); i != children.end(); ++i, ++child) {
		buildTreeItems(maxChild, child, *i, thisNode);
	}
}

}
}
