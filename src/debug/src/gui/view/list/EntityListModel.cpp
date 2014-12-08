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
	beginResetModel();
	endResetModel();
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
	if (section != 0)
		return QVariant();

	if (role == Qt::DisplayRole) {
		return tr("Entities");
	} else if (role == Qt::ToolTipRole) {
		return tr("The character id");
	}
	return QVariant();
}

QVariant EntityListModel::data(const QModelIndex &index, int role) const {
	const AIStateWorld* id = getEntity(index);
	if (role == Qt::DisplayRole) {
		if (index.column() == 0) {
			return id->getId();
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
