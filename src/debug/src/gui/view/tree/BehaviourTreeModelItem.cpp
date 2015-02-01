#include "BehaviourTreeModelItem.h"
#include "TreeViewCommon.h"

namespace ai {
namespace debug {

BehaviourTreeModelItem::BehaviourTreeModelItem(AIStateNode* node, BehaviourTreeModelItem* parent) :
		_node(node), _populated(false), _rowCount(0), _parent(parent) {
	for (const AIStateNode& node : _node->getChildren()) {
		_rows.push_back(new BehaviourTreeModelItem(const_cast<AIStateNode*>(&node), this));
	}
	// TODO: fill icon
}

BehaviourTreeModelItem::~BehaviourTreeModelItem() {
	qDeleteAll(_rows);
}

BehaviourTreeModelItem* BehaviourTreeModelItem::child(int row) {
	return _rows.value(row);
}

int BehaviourTreeModelItem::row() {
	if (_parent != nullptr)
		return _parent->_rows.indexOf(this);
	return 0;
}

QIcon BehaviourTreeModelItem::icon() {
	return _icon;
}

QVariant BehaviourTreeModelItem::data(int column) {
	if (column == 0)
		return QString::fromStdString(_node->getName());

	// the root item doesn't have a condition or anything like that...
	if (_parent == nullptr)
		return QVariant();

	switch (column) {
	case 1:
		return QString::fromStdString(_node->getCondition());
	case 2:
		return stateNames[_node->getStatus()];
	case 3:
		return QString::number(_node->getLastRun() / 1000);
	default:
		return QVariant();
	}
}

}
}
