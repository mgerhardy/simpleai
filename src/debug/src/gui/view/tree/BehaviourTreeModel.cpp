#include "BehaviourTreeModel.h"
#include "BehaviourTreeModelItem.h"

#include <QIcon>
#include <QDebug>

namespace ai {
namespace debug {

BehaviourTreeModel::BehaviourTreeModel(AINodeStaticResolver& resolver, QObject *parent) :
		QAbstractItemModel(parent), _rootItem(nullptr), _resolver(resolver) {
}

BehaviourTreeModel::~BehaviourTreeModel() {
	delete _rootItem;
}

QModelIndex BehaviourTreeModel::index(int row, int column, const QModelIndex &parent) const {
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	BehaviourTreeModelItem *parentItem;
	if (!parent.isValid())
		parentItem = _rootItem;
	else
		parentItem = item(parent);

	BehaviourTreeModelItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	return QModelIndex();
}

QModelIndex BehaviourTreeModel::parent(const QModelIndex &index) const {
	if (!index.isValid())
		return QModelIndex();

	BehaviourTreeModelItem *childItem = item(index);
	BehaviourTreeModelItem *parentItem = childItem->parent();
	if (parentItem == nullptr || parentItem == _rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int BehaviourTreeModel::rowCount(const QModelIndex &parent) const {
	BehaviourTreeModelItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = _rootItem;
	else
		parentItem = item(parent);

	return parentItem->childCount();
}

int BehaviourTreeModel::columnCount(const QModelIndex &parent) const {
	if (parent.isValid())
		return item(parent)->columnCount();
	return _rootItem->columnCount();
}

QVariant BehaviourTreeModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid())
		return QVariant();

	BehaviourTreeModelItem *nodeItem = item(index);

	if (role == Qt::DecorationRole) {
		if (index.column() == 0)
			return nodeItem->icon();
	} else if (role == Qt::TextColorRole) {
		const TreeNodeStatus status = nodeItem->node()->getStatus();
		switch (status) {
		case UNKNOWN:
		case CANNOTEXECUTE:
			return QColor(Qt::gray);
		case RUNNING:
		case FINISHED:
			return QColor(Qt::darkGreen);
		case FAILED:
		case EXCEPTION:
			return QColor(Qt::red);
		case MAX_TREENODESTATUS:
			break;
		}
		return QVariant();
	}

	if (role == Qt::DisplayRole)
		return nodeItem->data(index.column());
	return QVariant();
}

QVariant BehaviourTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return _rootItem->data(section);

	return QVariant();
}

void BehaviourTreeModel::setRootNode(AIStateNode* node) {
	beginResetModel();
	if (_rootItem) {
		delete _rootItem;
	}
	_rootItem = new BehaviourTreeModelItem(node, _resolver);
	endResetModel();
}

}
}
