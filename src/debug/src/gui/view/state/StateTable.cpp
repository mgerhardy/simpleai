#include "StateTable.h"
#include <QHeaderView>

namespace ai {
namespace debug {

StateTable::StateTable(AIDebugger& debugger) :
		QTableView(), _model(debugger), _debugger(debugger) {
	setFixedWidth(180);
	_proxyModel.setSourceModel(&_model);
	setModel(&_proxyModel);
	setAlternatingRowColors(true);
	setSortingEnabled(true);
	setSelectionMode(QAbstractItemView::NoSelection);
	verticalHeader()->hide();
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
}

StateTable::~StateTable() {
}

void StateTable::updateStateTable() {
	_model.update();
}

}
}
