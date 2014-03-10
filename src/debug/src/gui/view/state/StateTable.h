#pragma once

#include <QTableView>
#include <QStandardItemModel>

#include "AIDebugger.h"
#include <AI.h>

namespace ai {
namespace debug {

class StateTable: public QTableView {
private:
	QStandardItemModel *_model;
	AIDebugger& _debugger;
	const ai::AIStateTree* _selected;
public:
	StateTable(AIDebugger& debugger);
	virtual ~StateTable();

	void setSelected(const ai::AIStateTree* ai);
};

}
}
