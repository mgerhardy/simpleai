#include "AggroTableModel.h"
#include <QColor>

namespace ai {
namespace debug {

AggroTableModel::AggroTableModel(AIDebugger& debugger, QTableView *parent) :
		QAbstractTableModel(nullptr), _debugger(debugger), _parent(parent) {
}

AggroTableModel::~AggroTableModel() {
}

void AggroTableModel::update() {
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

int AggroTableModel::rowCount(const QModelIndex & /*parent*/) const {
	const std::vector<AIStateAggroEntry>& aggro = _debugger.getAggro();
	return aggro.size();
}

int AggroTableModel::columnCount(const QModelIndex & /*parent*/) const {
	return 2;
}

QVariant AggroTableModel::headerData(int section, Qt::Orientation orientation,
		int role) const {
	if (orientation != Qt::Horizontal)
		return QVariant();

	if (role == Qt::DisplayRole) {
		switch (section) {
		case 0:
			return tr("ID");
		case 1:
			return tr("Aggro");
		default:
			break;
		}
	}
	return QVariant();
}

QVariant AggroTableModel::data(const QModelIndex &index, int role) const {
	const std::vector<AIStateAggroEntry>& aggro = _debugger.getAggro();
	if (role == Qt::DisplayRole) {
		switch (index.column()) {
		case 0:
			return aggro.at(index.row()).id;
		case 1:
			return aggro.at(index.row()).aggro;
		default:
			break;
		}
	}
	return QVariant();
}

}
}
