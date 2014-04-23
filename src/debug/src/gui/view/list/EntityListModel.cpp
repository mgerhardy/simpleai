#include "EntityListModel.h"
#include <QColor>

namespace ai {
namespace debug {

EntityListModel::EntityListModel(AIDebugger& debugger, QTableView *parent) :
		QAbstractTableModel(parent), _debugger(debugger), _parent(parent) {
}

EntityListModel::~EntityListModel() {
}

void EntityListModel::update() {
	QModelIndexList selectedList = _parent->selectionModel()->selectedRows();
	QModelIndex scrollTopIndex = _parent->indexAt(QPoint(1, 1));
	QModelIndex scrollBottomIndex = _parent->indexAt(QPoint(1, _parent->height() - 1));

	beginResetModel();
	endResetModel();

	if (scrollBottomIndex.row() == -1) {
		_parent->scrollToBottom();
	} else {
		_parent->scrollTo(scrollTopIndex);
	}

	if (selectedList.count() > 0
			&& (selectedList.at(0).row() >= scrollTopIndex.row())
			&& (scrollBottomIndex.row() == -1
					|| scrollBottomIndex.row() > selectedList.at(0).row())) {
		_parent->selectRow(selectedList.at(0).row());
	}
}

int EntityListModel::rowCount(const QModelIndex & /*parent*/) const {
	return getEntities().size();
}

int EntityListModel::columnCount(const QModelIndex & /*parent*/) const {
	return 1;
}

QVariant EntityListModel::headerData(int section, Qt::Orientation orientation,
		int role) const {
	if (orientation != Qt::Horizontal)
		return QVariant();

	if (role == Qt::DisplayRole) {
		switch (section) {
		case 0:
			return tr("Entities");
		default:
			break;
		}
	} else if (role == Qt::ToolTipRole) {
		switch (section) {
		case 0:
			return tr("The character id");
		default:
			break;
		}
	}
	return QVariant();
}

QVariant EntityListModel::data(const QModelIndex &index, int role) const {
	const AIStateWorld* id = getEntity(index);
	if (role == Qt::DisplayRole) {
		switch (index.column()) {
		case 0:
			return id->getId();
		default:
			break;
		}
	} else if (role == Qt::BackgroundColorRole) {
		if (_debugger.isSelected(*id)) {
			return QColor(Qt::gray);
		}
	}
	return QVariant();
}

}
}
