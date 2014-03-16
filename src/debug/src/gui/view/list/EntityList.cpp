#include "EntityList.h"

namespace ai {
namespace debug {

EntityList::EntityList(AIDebugger& debugger) :
		QTableView(), _debugger(debugger) {
	setFixedWidth(140);
	_model = new QStandardItemModel(0, 1, this);
	_model->setHorizontalHeaderItem(0, new QStandardItem(tr("Entity")));
	setModel(_model);
}

EntityList::~EntityList() {
}

void EntityList::updateEntityList() {
	_model->clear();
	const AIDebugger::Entities& entities = _debugger.getEntities();
	for (AIDebugger::EntitiesIter i = entities.begin(); i != entities.end(); ++i) {
		const AIStateWorld& ai = *i;
		const ai::CharacterId& id = ai.getId();
		const QString idStr = QString::number(id);
		QStandardItem *row = new QStandardItem(idStr);
		if (_debugger.isSelected(ai)) {
			//row->setBackground();
		}
		_model->appendRow(row);
	}
	_model->sort(0);
}

// _aiDebugger.select(_tree);

}
}
