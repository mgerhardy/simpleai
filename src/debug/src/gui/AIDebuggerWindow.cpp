#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QSplitter>

#include "AIDebugger.h"
#include "AIDebuggerWindow.h"
#include "ConnectDialog.h"
#include "StateTable.h"
#include "EntityList.h"
#include "AggroTable.h"
#include "MapView.h"
#include "ZoomFrame.h"
#include "NodeTreeView.h"

namespace ai {
namespace debug {

AIDebuggerWindow::AIDebuggerWindow(AIDebugger& debugger) :
		QMainWindow(), _debugger(debugger) {
	createView();
	createActions();
	createMenus();

	_statusBarLabel = new QLabel(tr("not connected"));
	_selectedLabel = new QLabel(tr("nothing selected"));
	statusBar()->addWidget(_statusBarLabel);
	statusBar()->addWidget(_selectedLabel);

	_toolbar = addToolBar("");
	_toolbar->setMovable(false);
	_toolbar->setFloatable(false);
	_toolbar->addAction(_connectAction);
	_toolbar->addAction(_pauseAction);
	addToolBar(Qt::TopToolBarArea, _toolbar);

	resize(1024, 768);
	setWindowTitle(tr("AI Debugger"));

	connect(&_debugger, SIGNAL(onPause(bool)), SLOT(setPause(bool)));

	// TODO: react on network input from the AIDebugger and remove this timer
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
	timer->start(20);
}

AIDebuggerWindow::~AIDebuggerWindow() {
	delete _nodeTreeFrame;
	delete _mapFrame;
	delete _entityList;
	delete _statusBarLabel;
	delete _selectedLabel;
	delete _stateTable;
	delete _aggroTable;
}

void AIDebuggerWindow::createView() {
	QWidget *widget = new QWidget;
	setCentralWidget(widget);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setMargin(2);
	layout->addWidget(createTopWidget());
	layout->addWidget(createBottomWidget());
	widget->setLayout(layout);
}

QWidget *AIDebuggerWindow::createTopWidget() {
	QSplitter *splitter = new QSplitter;

	_mapWidget = _debugger.createMapWidget();
	_mapFrame = new ZoomFrame(_mapWidget);

	_entityList = new EntityList(_debugger);
	_entityList->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);

	splitter->addWidget(_mapFrame);
	splitter->addWidget(_entityList);
	return splitter;
}

QWidget *AIDebuggerWindow::createBottomWidget() {
	QSplitter *splitter = new QSplitter;
	_nodeTree = new NodeTreeView(_debugger);
	_nodeTreeFrame = new ZoomFrame(_nodeTree);
	_aggroTable = new AggroTable(_debugger);
	_stateTable = new StateTable(_debugger);
	splitter->addWidget(_nodeTreeFrame);
	splitter->addWidget(_aggroTable);
	splitter->addWidget(_stateTable);
	return splitter;
}

void AIDebuggerWindow::setPause(bool pause) {
	if (pause) {
		_pauseAction->setIcon(QIcon(":/images/pause.png"));
	} else {
		_pauseAction->setIcon(QIcon(":/images/continue.png"));
	}
}

void AIDebuggerWindow::requestPause() {
	_debugger.togglePause();
}

void AIDebuggerWindow::connectToAIServer() {
	ConnectDialog d;
	const int state = d.run();
	const short port = d.getPort();
	const QString& hostname = d.getHostname();
	if (state == QDialog::Accepted && _debugger.connectToAIServer(hostname, port)) {
		_statusBarLabel->setText(tr("connected to %1:%2").arg(hostname).arg(port));
	} else {
		_statusBarLabel->setText(tr("not connected"));
	}
}

void AIDebuggerWindow::about() {
	QMessageBox::about(this, tr("About"), tr("AI debug visualization for libsimpleai.<br />Grab the latest version at <a href=\"https://github.com/mgerhardy/simpleai\">github</a>"));
}

void AIDebuggerWindow::tick() {
	const ai::CharacterId& id = _debugger.getSelected();
	if (id == -1) {
		_selectedLabel->setText(tr("nothing selected"));
	} else {
		_selectedLabel->setText(tr("selected %1").arg(id));
	}
	_stateTable->updateStateTable();
	_entityList->updateEntityList();
	_mapWidget->updateMapView();
	_nodeTree->updateTreeWidget();
	_aggroTable->updateAggroTable();
}

void AIDebuggerWindow::createMenus() {
	_fileMenu = menuBar()->addMenu(tr("&File"));
	_fileMenu->addAction(_connectAction);
	_fileMenu->addSeparator();
	_fileMenu->addAction(_exitAction);

	_helpMenu = menuBar()->addMenu(tr("&Help"));
	_helpMenu->addAction(_aboutAction);
}

void AIDebuggerWindow::createActions() {
	_connectAction = new QAction(tr("C&onnect"), this);
	_connectAction->setShortcuts(QKeySequence::Open);
	_connectAction->setStatusTip(tr("Connect to AI server"));
	_connectAction->setIcon(QIcon(":/images/connect.png"));
	connect(_connectAction, SIGNAL(triggered()), this, SLOT(connectToAIServer()));

	_pauseAction = new QAction(tr("Pause"), this);
	_pauseAction->setStatusTip(tr("Freeze the ai controlled entities"));
	_pauseAction->setIcon(QIcon(":/images/continue.png"));
	connect(_pauseAction, SIGNAL(triggered()), this, SLOT(requestPause()));

	_exitAction = new QAction(tr("E&xit"), this);
	_exitAction->setShortcuts(QKeySequence::Quit);
	_exitAction->setStatusTip(tr("Exit the application"));
	_exitAction->setIcon(QIcon(":/images/exit.png"));
	connect(_exitAction, SIGNAL(triggered()), this, SLOT(close()));

	_aboutAction = new QAction(tr("&About"), this);
	_aboutAction->setStatusTip(tr("Show the application's About box"));
	_aboutAction->setIcon(QIcon(":/images/about.png"));
	connect(_aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

QLabel *AIDebuggerWindow::createLabel(const QString &text) const {
	QLabel *label = new QLabel(text);
	label->setAlignment(Qt::AlignCenter);
	label->setMargin(2);
	label->setFrameStyle(QFrame::Box | QFrame::Sunken);
	return label;
}

}
}
