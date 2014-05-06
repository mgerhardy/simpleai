#include "AggroTable.h"
#include <QHeaderView>

namespace ai {
namespace debug {

AggroTable::AggroTable(AIDebugger& debugger) :
		QTableView(), _model(debugger, this), _debugger(debugger) {
	setFixedWidth(160);
	_proxyModel.setSourceModel(&_model);
	setModel(&_proxyModel);
	setAlternatingRowColors(true);
	resizeColumnsToContents();
	setSortingEnabled(false);
	setSelectionMode(QAbstractItemView::NoSelection);
	verticalHeader()->hide();
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
	horizontalHeader()->setStretchLastSection(true);
}

AggroTable::~AggroTable() {
}

void AggroTable::updateAggroTable() {
	_model.update();
}

}
}
