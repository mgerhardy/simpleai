#include "StateTableModel.h"
#include <QColor>

namespace ai {
namespace debug {

StateTableModel::StateTableModel(AIDebugger& debugger) :
		QAbstractTableModel(nullptr), _debugger(debugger) {
}

StateTableModel::~StateTableModel() {
}

void StateTableModel::update() {
	beginResetModel();
	endResetModel();
}

int StateTableModel::rowCount(const QModelIndex & /*parent*/) const {
	// TODO:
	return 0;
}

int StateTableModel::columnCount(const QModelIndex & /*parent*/) const {
	return 2;
}

QVariant StateTableModel::headerData(int section, Qt::Orientation orientation,
		int role) const {
	if (orientation != Qt::Horizontal)
		return QVariant();

	if (role == Qt::DisplayRole) {
		switch (section) {
		case 0:
			return tr("Key");
		case 1:
			return tr("Value");
		default:
			break;
		}
	}
	return QVariant();
}

QVariant StateTableModel::data(const QModelIndex &index, int role) const {
	// TODO:
	if (role == Qt::DisplayRole) {
		switch (index.column()) {
		case 0:
			return "";
		case 1:
			return "";
		default:
			break;
		}
	}
	return QVariant();
}

}
}
