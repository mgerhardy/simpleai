#include "BehaviourTreeModelItem.h"
#include "TreeViewCommon.h"
#include "AINodeStaticResolver.h"
#include <QFile>

namespace ai {
namespace debug {

BehaviourTreeModelItem::BehaviourTreeModelItem(AIStateNode* node, AINodeStaticResolver& resolver, BehaviourTreeModelItem* parent) :
		_node(node), _staticNodeData(resolver.get(node->getNodeId())), _populated(false), _rowCount(0), _parent(parent) {
	for (const AIStateNode& node : _node->getChildren()) {
		_rows.push_back(new BehaviourTreeModelItem(const_cast<AIStateNode*>(&node), resolver, this));
	}
	const QString path = ":/images/" + QString::fromStdString(_staticNodeData.getType()).toLower() + ".png";
	if (QFile::exists(path))
		_icon = QIcon(path);
	else
		_icon = QIcon(":/images/node.png");
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
		return QString::fromStdString(_staticNodeData.getName());

	// the root item doesn't have a condition or anything like that...
	if (_parent == nullptr) {
		switch (column) {
		case 1:
			return QObject::tr("Condition");
		case 2:
			return QObject::tr("State");
		case 3:
			return QObject::tr("Last run");
		}
		return QVariant();
	}

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
