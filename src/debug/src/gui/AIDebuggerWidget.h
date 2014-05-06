#pragma once

#include <QtGui>
#include <QLabel>
#include <QToolBar>
#include <QStatusBar>
#include <QComboBox>
#include <QMenu>

namespace ai {
namespace debug {

class StateTable;
class EntityList;
class AggroTable;
class NodeTreeView;
class ZoomFrame;
class MapView;
class AIDebugger;

/**
 * @brief The widget that represents the whole ai debugger
 */
class AIDebuggerWidget: public QWidget {
Q_OBJECT
public:
	AIDebuggerWidget(AIDebugger& debugger);
	virtual ~AIDebuggerWidget();

	void connectToAIServer(const QString& hostname, short port);

	/**
	 * @brief Call this with an existing @c QStatusBar to add additional ai debugger information to it
	 */
	void contributeToStatusBar(QStatusBar* statusBar);
	/**
	 * @brief Call this with an existing @c QToolbar to add addition ai debugger buttons to it
	 */
	void contributeToToolBar(QToolBar* toolBar);
	/**
	 * @brief Call this with an existing @c QToolbar to add addition ai debugger entries to it
	 */
	void contributeToFileMenu(QMenu *fileMenu);
	/**
	 * @brief Call this with an existing @c QToolbar to add addition ai debugger entries to it
	 */
	void contributeToHelpMenu(QMenu *helpMenu);

	/**
	 * @brief If you let the ai debugger contribute to the status bar, call if to remove the contribution
	 */
	void removeFromStatusBar(QStatusBar* statusBar);
	/**
	 * @brief If you let the ai debugger contribute to the tool bar, call if to remove the contribution
	 */
	void removeFromToolBar(QToolBar* toolBar);
	/**
	 * @brief If you let the ai debugger contribute to the file menu, call if to remove the contribution
	 */
	void removeFromFileMenu(QMenu *fileMenu);
	/**
	 * @brief If you let the ai debugger contribute to the help menu, call if to remove the contribution
	 */
	void removeFromHelpMenu(QMenu *helpMenu);

private slots:
	/**
	 * @brief This must be called in order to update all the widgets
	 */
	void tick();
	void about();
	void connectToAIServer();
	void requestPause();
	void requestStep();
	void requestReset();
	void setPause(bool pause);
	void onDisconnect();
	void change(const QString &);
	void setNames(const std::vector<std::string>& names);

private:
	void createView();
	void createActions();

	QWidget *createTopWidget();
	QWidget *createBottomWidget();
	QLabel *createLabel(const QString &text) const;

	NodeTreeView *_nodeTree;
	ZoomFrame *_nodeTreeFrame;
	StateTable *_stateTable;
	ZoomFrame *_mapFrame;
	MapView *_mapWidget;
	EntityList *_entityList;
	AggroTable *_aggroTable;
	QAction *_connectAction;
	QAction *_pauseAction;
	QAction *_stepAction;
	QAction *_resetAction;
	QAction *_aboutAction;
	QLabel *_statusBarLabel;
	QLabel *_selectedLabel;
	QComboBox *_namesComboBox;

	AIDebugger& _debugger;
	QString _name;
};

}
}
