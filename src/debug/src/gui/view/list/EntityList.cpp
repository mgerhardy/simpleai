#include "EntityList.h"
#include <QHeaderView>

namespace ai {
namespace debug {

EntityList::EntityList(AIDebugger& debugger) :
		QTableView(), _model(debugger), _debugger(debugger) {
	setFixedWidth(100);
	_proxyModel.setSourceModel(&_model);
	setModel(&_proxyModel);
	setAlternatingRowColors(true);
	setSortingEnabled(true);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	verticalHeader()->hide();

	connect(selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(selectEntity(QModelIndex,QModelIndex)));
}

EntityList::~EntityList() {
}

void EntityList::updateEntityList() {
	_model.update();
}

void EntityList::selectEntity(const QModelIndex &current, const QModelIndex &previous) {
	Q_UNUSED(previous);
	const AIStateWorld* state = _model.getEntity(_proxyModel.mapToSource(current));
	if (state == nullptr) {
		qDebug() << "Error selecting entity from list for index " << current;
		return;
	}
	_debugger.select(*state);
}


}
}
