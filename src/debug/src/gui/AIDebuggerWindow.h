#pragma once

#include <QtGui>
#include <QLabel>
#include <QMainWindow>
#include <QToolBar>

namespace ai {

class AIDebugger;

namespace debug {

class StateTable;
class EntityList;
class AggroTable;
class NodeTreeView;
class MapFrame;
class MapView;

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

	NodeTreeView *_nodeTree;
	StateTable *_stateTable;
	MapFrame *_mapFrame;
	MapView *_mapWidget;
	EntityList *_entityList;
	AggroTable *_aggroTable;
	QMenu *_fileMenu;
	QMenu *_helpMenu;
	QAction *_connectAction;
	QAction *_exitAction;
	QAction *_aboutAction;
	QLabel *_statusBarLabel;
	QLabel *_selectedLabel;
	QToolBar *_toolbar;

	AIDebugger& _debugger;
};

}
}
