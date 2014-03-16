#pragma once

#include <QTableView>
#include "EntityListModel.h"
#include <QSortFilterProxyModel>

#include "AIDebugger.h"

namespace ai {
namespace debug {

/**
 * @brief Shows a list of all entities that are handled on the server we are connected to
 */
class EntityList: public QTableView {
Q_OBJECT
private:
	EntityListModel _model;
	QSortFilterProxyModel _proxyModel;
	AIDebugger& _debugger;
private slots:
	void selectEntity(const QModelIndex &current, const QModelIndex &previous);
public:
	EntityList(AIDebugger& debugger);
	virtual ~EntityList();

	void updateEntityList();
};

}
}
