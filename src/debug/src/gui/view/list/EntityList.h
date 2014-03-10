#pragma once

#include <QTableView>
#include <QStandardItemModel>

#include "AIDebugger.h"

namespace ai {
namespace debug {

class EntityList: public QTableView {
private:
	QStandardItemModel *_model;
	AIDebugger& _debugger;
public:
	EntityList(AIDebugger& debugger);
	virtual ~EntityList();

	void updateEntityList();
};

}
}
