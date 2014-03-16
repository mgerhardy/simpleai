#include "EntityListModel.h"
#include <QColor>

namespace ai {
namespace debug {

EntityListModel::EntityListModel(AIDebugger& debugger) :
		QAbstractTableModel(nullptr), _debugger(debugger) {
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
