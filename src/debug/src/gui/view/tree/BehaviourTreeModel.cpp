#include "BehaviourTreeModel.h"
#include "BehaviourTreeModelItem.h"

#include <QIcon>
#include <QDebug>

namespace ai {
namespace debug {

BehaviourTreeModel::BehaviourTreeModel(QObject *parent) :
		QAbstractItemModel(parent), _rootItem(nullptr) {
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
		parentItem = static_cast<BehaviourTreeModelItem*>(parent.internalPointer());

	BehaviourTreeModelItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	return QModelIndex();
}

QModelIndex BehaviourTreeModel::parent(const QModelIndex &index) const {
	if (!index.isValid())
		return QModelIndex();

	BehaviourTreeModelItem *childItem = static_cast<BehaviourTreeModelItem*>(index.internalPointer());
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
		parentItem = static_cast<BehaviourTreeModelItem*>(parent.internalPointer());

	return parentItem->childCount();
}

int BehaviourTreeModel::columnCount(const QModelIndex &parent) const {
	if (parent.isValid())
		return static_cast<BehaviourTreeModelItem*>(parent.internalPointer())->columnCount();
	return _rootItem->columnCount();
}

QVariant BehaviourTreeModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	BehaviourTreeModelItem *item = static_cast<BehaviourTreeModelItem*>(index.internalPointer());
	return item->data(index.column());
}

QVariant BehaviourTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return _rootItem->data(section);

	return QVariant();
}

void BehaviourTreeModel::setRootNode(AIStateNode* node) {
	if (_rootItem)
		delete _rootItem;
	_rootItem = new BehaviourTreeModelItem(node);
}

}
}
