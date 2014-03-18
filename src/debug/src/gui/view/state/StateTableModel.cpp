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
	const CharacterAttributes& a = _debugger.getAttributes();
	_list.clear();
	for (CharacterAttributes::const_iterator i = a.begin(); i != a.end(); ++i) {
		_list.push_back(i->first);
	}
	endResetModel();
}

int StateTableModel::rowCount(const QModelIndex & /*parent*/) const {
	return _list.size();
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
			return QString::fromStdString(_list[index.row()]);
		case 1: {
			const CharacterAttributes& a = _debugger.getAttributes();
			const std::string& key = _list[index.row()];
			return QString::fromStdString(a.find(key)->second);
		}
		default:
			break;
		}
	}
	return QVariant();
}

}
}
