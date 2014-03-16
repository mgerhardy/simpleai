#include "AggroTable.h"
#include <QHeaderView>

namespace ai {
namespace debug {

AggroTable::AggroTable(AIDebugger& debugger) :
		QTableView(), _model(debugger), _debugger(debugger) {
	setFixedWidth(120);
	_proxyModel.setSourceModel(&_model);
	setModel(&_proxyModel);
	setAlternatingRowColors(true);
	setSortingEnabled(true);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	verticalHeader()->hide();
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
}

AggroTable::~AggroTable() {
}

void AggroTable::updateAggroTable() {
	_model.update();
}

}
}
