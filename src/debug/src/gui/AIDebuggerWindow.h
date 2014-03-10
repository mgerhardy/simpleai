#pragma once

#include <QtGui>
#include <QLabel>
#include <QMainWindow>

#include "AIDebugger.h"
#include "StateTable.h"
#include "EntityList.h"
#include "NodeTreeWidget.h"

namespace ai {
namespace debug {

class AIDebuggerWindow: public QMainWindow {
Q_OBJECT
public:
	AIDebuggerWindow(AIDebugger& debugger);
	virtual ~AIDebuggerWindow();

private slots:
	void about();
	void tick();
	void connectToAIServer();

private:
	void createView();
	void createActions();
	void createMenus();

	QWidget *createTopWidget();
	QWidget *createBottomWidget();
	QLabel *createLabel(const QString &text) const;

	NodeTreeWidget *_nodeTree;
	StateTable *_stateTable;
	MapView *_mapView;
	EntityList *_entityList;
	QMenu *_fileMenu;
	QMenu *_helpMenu;
	QAction *_connectAction;
	QAction *_exitAction;
	QAction *_aboutAction;
	QLabel *_statusBarLabel;

	AIDebugger& _debugger;
};

}
}
