#include "AIDebuggerWindow.h"
#include "ConnectDialog.h"
#include <AI.h>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>

namespace ai {
namespace debug {

AIDebuggerWindow::AIDebuggerWindow(AIDebugger& debugger) :
		QMainWindow(), _debugger(debugger) {
	createView();
	createActions();
	createMenus();

	_statusBarLabel = new QLabel(tr("not connected"));
	statusBar()->addWidget(_statusBarLabel);

	resize(1024, 768);
	setWindowTitle(tr("AI Debugger"));

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
	timer->start(500);
}

AIDebuggerWindow::~AIDebuggerWindow() {
	delete _mapView;
	delete _entityList;
	delete _statusBarLabel;
	delete _nodeTree;
	delete _stateTable;
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
	QWidget *topWidget = new QWidget;
	QHBoxLayout *topLayout = new QHBoxLayout;

	_mapView = _debugger.createMapWidget();
	_mapView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	_entityList = new EntityList(_debugger);
	_entityList->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);

	topLayout->addWidget(_mapView);
	topLayout->addWidget(_entityList);
	topWidget->setLayout(topLayout);
	return topWidget;
}

QWidget *AIDebuggerWindow::createBottomWidget() {
	QWidget *bottomWidget = new QWidget;
	QHBoxLayout *bottomLayout = new QHBoxLayout;

	_nodeTree = new NodeTreeWidget(_debugger);
	_nodeTree->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	_stateTable = new StateTable(_debugger);
	_stateTable->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);

	bottomLayout->addWidget(_nodeTree);
	bottomLayout->addWidget(_stateTable);
	bottomWidget->setLayout(bottomLayout);
	return bottomWidget;
}

void AIDebuggerWindow::connectToAIServer() {
	ConnectDialog d;
	d.run();
	const short port = d.getPort();
	const QString hostname = d.getHostname();
	if (_debugger.connectToAIServer(hostname, port)) {
		_statusBarLabel->setText(tr("connected to %1:%2").arg(hostname).arg(port));
	} else {
		_statusBarLabel->setText(tr("not connected"));
	}
}

void AIDebuggerWindow::about() {
	QMessageBox::about(this, tr("About"), tr("AI debug visualization for libsimpleai."));
}

void AIDebuggerWindow::tick() {
	const ai::AIStateTree* selected = _debugger.getSelected();
	_stateTable->setSelected(selected);
	_entityList->updateEntityList();
	_mapView->updateMapView();
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
	connect(_connectAction, SIGNAL(triggered()), this, SLOT(connectToAIServer()));

	_exitAction = new QAction(tr("E&xit"), this);
	_exitAction->setShortcuts(QKeySequence::Quit);
	_exitAction->setStatusTip(tr("Exit the application"));
	connect(_exitAction, SIGNAL(triggered()), this, SLOT(close()));

	_aboutAction = new QAction(tr("&About"), this);
	_aboutAction->setStatusTip(tr("Show the application's About box"));
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
