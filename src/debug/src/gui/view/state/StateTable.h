#pragma once

#include <QTableView>
#include <QStandardItemModel>

#include "AIDebugger.h"
#include <AI.h>

namespace ai {
namespace debug {

/**
 * @brief Shows a key value pair of values for the selected entity
 */
class StateTable: public QTableView {
private:
	QStandardItemModel *_model;
	AIDebugger& _debugger;
public:
	StateTable(AIDebugger& debugger);
	virtual ~StateTable();

	void updateStateTable();
};

}
}
