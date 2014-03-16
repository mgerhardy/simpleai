#include "StateTable.h"

namespace ai {
namespace debug {

StateTable::StateTable(AIDebugger& debugger) :
		QTableView(), _debugger(debugger) {
	setFixedWidth(140);
	_model = new QStandardItemModel(0, 1, this);
	_model->setHorizontalHeaderItem(0, new QStandardItem(tr("State")));
	setModel(_model);
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
}

StateTable::~StateTable() {
}

void StateTable::updateStateTable() {
	const CharacterId& id = _debugger.getSelected();
	if (id == -1) {
		_model->clear();
		update();
		return;
	}

	update();
}

}
}
