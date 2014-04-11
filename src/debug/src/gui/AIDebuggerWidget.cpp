#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QSplitter>

#include "AIDebugger.h"
#include "AIDebuggerWidget.h"
#include "ConnectDialog.h"
#include "StateTable.h"
#include "EntityList.h"
#include "AggroTable.h"
#include "MapView.h"
#include "ZoomFrame.h"
#include "NodeTreeView.h"

namespace ai {
namespace debug {

AIDebuggerWidget::AIDebuggerWidget(AIDebugger& debugger) :
		QWidget(), _debugger(debugger) {
	createView();
	createActions();

	_statusBarLabel = new QLabel(tr("not connected"));
	_selectedLabel = new QLabel(tr("nothing selected"));

	connect(_namesComboBox, SIGNAL(activated(const QString&)), SLOT(change(const QString&)));
	connect(&_debugger, SIGNAL(onPause(bool)), SLOT(setPause(bool)));
	connect(&_debugger, SIGNAL(onNamesReceived(const std::vector<std::string>&)), SLOT(setNames(const std::vector<std::string>&)));
	connect(&_debugger, SIGNAL(disconnected()), SLOT(onDisconnect()));

	// TODO: react on network input from the AIDebugger and remove this timer
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
	timer->start(20);
}

AIDebuggerWidget::~AIDebuggerWidget() {
	delete _nodeTreeFrame;
	delete _mapFrame;
	delete _entityList;
	delete _statusBarLabel;
	delete _selectedLabel;
	delete _stateTable;
	delete _aggroTable;
	delete _pauseAction;
	delete _resetAction;
	delete _stepAction;
	delete _connectAction;
	delete _aboutAction;
	delete _namesComboBox;
}

void AIDebuggerWidget::setNames(const std::vector<std::string>& names) {
	_names = names;
	for (std::vector<std::string>::const_iterator i = _names.begin(); i != _names.end(); ++i) {
		_namesComboBox->addItem(QString::fromStdString(*i));
	}
	_namesComboBox->setEnabled(!_names.empty());
}

void AIDebuggerWidget::contributeToStatusBar(QStatusBar* statusBar) {
	statusBar->addWidget(_statusBarLabel);
	statusBar->addWidget(_selectedLabel);
}

void AIDebuggerWidget::contributeToToolBar(QToolBar* toolBar) {
	toolBar->addAction(_connectAction);
	toolBar->addAction(_pauseAction);
	toolBar->addAction(_stepAction);
	toolBar->addAction(_resetAction);
}

void AIDebuggerWidget::contributeToFileMenu(QMenu *fileMenu) {
	fileMenu->addAction(_connectAction);
}

void AIDebuggerWidget::contributeToHelpMenu(QMenu *helpMenu) {
	helpMenu->addAction(_aboutAction);
}

void AIDebuggerWidget::removeFromStatusBar(QStatusBar* statusBar) {
	statusBar->removeWidget(_statusBarLabel);
	statusBar->removeWidget(_selectedLabel);
}

void AIDebuggerWidget::removeFromToolBar(QToolBar* toolBar) {
	toolBar->removeAction(_connectAction);
	toolBar->removeAction(_pauseAction);
	toolBar->removeAction(_stepAction);
	toolBar->removeAction(_resetAction);
}

void AIDebuggerWidget::removeFromFileMenu(QMenu *fileMenu) {
	fileMenu->removeAction(_connectAction);
}

void AIDebuggerWidget::removeFromHelpMenu(QMenu *helpMenu) {
	helpMenu->removeAction(_aboutAction);
}

void AIDebuggerWidget::createView() {
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setMargin(2);
	layout->addWidget(createTopWidget());
	layout->addWidget(createBottomWidget());
	setLayout(layout);
}

QWidget *AIDebuggerWidget::createTopWidget() {
	QSplitter *splitter = new QSplitter;

	_mapWidget = _debugger.createMapWidget();
	_mapFrame = new ZoomFrame(_mapWidget);

	_entityList = new EntityList(_debugger);
	_namesComboBox = new QComboBox();
	_namesComboBox->setFixedWidth(_entityList->width());
	_namesComboBox->addItem(tr("None"));

	splitter->addWidget(_mapFrame);

	QVBoxLayout *vbox = new QVBoxLayout();
	vbox->setMargin(0);
	vbox->addWidget(_namesComboBox);
	vbox->addWidget(_entityList);

	QWidget *widget = new QWidget();
	widget->setFixedWidth(_entityList->width());
	widget->setLayout(vbox);
	splitter->addWidget(widget);

	return splitter;
}

QWidget *AIDebuggerWidget::createBottomWidget() {
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

void AIDebuggerWidget::onDisconnect() {
	_statusBarLabel->setText(tr("not connected"));
}

void AIDebuggerWidget::change(const QString& name) {
	_debugger.change(name);
}

void AIDebuggerWidget::setPause(bool pause) {
	if (pause) {
		_pauseAction->setIcon(QIcon(":/images/continue.png"));
	} else {
		_pauseAction->setIcon(QIcon(":/images/pause.png"));
	}
}

void AIDebuggerWidget::requestStep() {
	_debugger.step();
}

void AIDebuggerWidget::requestReset() {
	_debugger.reset();
}

void AIDebuggerWidget::requestPause() {
	_debugger.togglePause();
}

void AIDebuggerWidget::connectToAIServer(const QString& hostname, short port) {
	if (_debugger.connectToAIServer(hostname, port)) {
		_statusBarLabel->setText(tr("connected to %1:%2").arg(hostname).arg(port));
	} else {
		_statusBarLabel->setText(tr("not connected"));
	}
}

void AIDebuggerWidget::connectToAIServer() {
	ConnectDialog d;
	const int state = d.run();
	if (state != QDialog::Accepted) {
		_statusBarLabel->setText(tr("not connected"));
		return;
	}
	const short port = d.getPort();
	const QString& hostname = d.getHostname();
	connectToAIServer(hostname, port);
}

void AIDebuggerWidget::about() {
	QMessageBox::about(this, tr("About"), tr("AI debug visualization for libsimpleai.<br />Grab the latest version at <a href=\"https://github.com/mgerhardy/simpleai\">github</a>"));
}

void AIDebuggerWidget::tick() {
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

void AIDebuggerWidget::createActions() {
	_connectAction = new QAction(tr("C&onnect"), this);
	_connectAction->setShortcuts(QKeySequence::Open);
	_connectAction->setStatusTip(tr("Connect to AI server"));
	_connectAction->setIcon(QIcon(":/images/connect.png"));
	connect(_connectAction, SIGNAL(triggered()), this, SLOT(connectToAIServer()));

	_pauseAction = new QAction(tr("Pause"), this);
	_pauseAction->setStatusTip(tr("Freeze the ai controlled entities"));
	_pauseAction->setIcon(QIcon(":/images/pause.png"));
	connect(_pauseAction, SIGNAL(triggered()), this, SLOT(requestPause()));

	_stepAction = new QAction(tr("Step"), this);
	_stepAction->setStatusTip(tr("Performs one step while ai is in pause mode"));
	_stepAction->setIcon(QIcon(":/images/step.png"));
	connect(_stepAction, SIGNAL(triggered()), this, SLOT(requestStep()));

	_resetAction = new QAction(tr("Reset"), this);
	_resetAction->setStatusTip(tr("Resets the states of the ai"));
	_resetAction->setIcon(QIcon(":/images/reset.png"));
	connect(_resetAction, SIGNAL(triggered()), this, SLOT(requestReset()));

	_aboutAction = new QAction(tr("&About"), this);
	_aboutAction->setStatusTip(tr("Show the application's About box"));
	_aboutAction->setIcon(QIcon(":/images/about.png"));
	connect(_aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

QLabel *AIDebuggerWidget::createLabel(const QString &text) const {
	QLabel *label = new QLabel(text);
	label->setAlignment(Qt::AlignCenter);
	label->setMargin(2);
	label->setFrameStyle(QFrame::Box | QFrame::Sunken);
	return label;
}

}
}
