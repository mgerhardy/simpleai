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
	const AIDebugger::Entities& entities = _debugger.getEntities();
	for (AIDebugger::EntitiesIter i = entities.begin(); i != entities.end(); ++i) {
		const ai::CharacterId& id = i->getId();
		const QString idStr = QString::number(id);
		QList<QStandardItem*> list = _model->findItems(idStr);
		if (!list.empty())
			continue;
		QStandardItem *row = new QStandardItem(idStr);
		_model->appendRow(row);
	}
	_model->sort(0);

	const ai::AIStateTree* ai = _debugger.getSelected();
	if (ai != nullptr) {
		const QString idStr = QString::number(ai->getId());
		QList<QStandardItem*> list = _model->findItems(idStr);
		selectRow(list.at(0)->row());
	}
}

}
}
