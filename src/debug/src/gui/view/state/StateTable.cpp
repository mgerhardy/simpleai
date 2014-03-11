#include "StateTable.h"

namespace ai {
namespace debug {

StateTable::StateTable(AIDebugger& debugger) :
		QTableView(), _debugger(debugger), _selected(nullptr) {
	setFixedWidth(140);
	_model = new QStandardItemModel(0, 1, this);
	_model->setHorizontalHeaderItem(0, new QStandardItem(tr("State")));
	setModel(_model);
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
}

StateTable::~StateTable() {
}

void StateTable::setSelected(const ai::AIStateTree* ai) {
	_selected = ai;

	if (_selected == nullptr) {
		_model->clear();
		update();
		return;
	}

	update();
}

}
}
