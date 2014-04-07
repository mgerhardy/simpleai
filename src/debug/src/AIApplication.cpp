#include "AIApplication.h"
#include "AIDebugger.h"
#include "AIDebuggerWidget.h"
#include "Version.h"

#include <QList>
#include <QString>
#include <QToolBar>
#include <QMenuBar>

namespace ai {
namespace debug {

AIApplication::AIApplication(int argc, char** argv) :
		QApplication(argc, argv) {
#ifdef Q_WS_X11
	QApplication::setGraphicsSystem(QLatin1String("raster"));
#endif
	setOrganizationDomain("simpleai");
	setApplicationName("simpleai");
	setApplicationVersion(VERSION);
#ifdef Q_WS_MAC
	a.setAttribute(Qt::AA_DontShowIconsInMenus);
#endif

	_debugger = new AIDebugger();
	_widget = new AIDebuggerWidget(*_debugger);

	_window.setCentralWidget(_widget);

	QToolBar* _toolbar = _window.addToolBar("");
	_toolbar->setMovable(false);
	_toolbar->setFloatable(false);
	_window.addToolBar(Qt::TopToolBarArea, _toolbar);

	_widget->contributeToStatusBar(_window.statusBar());
	_widget->contributeToToolBar(_toolbar);
	QMenu* menuBar = _window.menuBar()->addMenu(tr("&File"));
	_widget->contributeToFileMenu(menuBar);
	_widget->contributeToHelpMenu(_window.menuBar()->addMenu(tr("&Help")));
	_window.show();

	const QList<QString>& args = QCoreApplication::arguments();
	if (args.size() == 3) {
		const QString hostname = args.at(1);
		const short port = args.at(2).toShort();
		qDebug() << "connect to " << hostname << " on port " << port;
		_widget->connectToAIServer(hostname, port);
	}
}

AIApplication::~AIApplication() {
	delete _debugger;
	delete _widget;
}

}
}
