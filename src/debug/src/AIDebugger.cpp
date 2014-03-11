#include "AIDebugger.h"
#include "AIDebuggerWindow.h"
#include <tree/loaders/lua/LUATreeLoader.h>
#include <server/IProtocolMessage.h>
#include <server/ProtocolMessageFactory.h>
#include <server/ProtocolHandlerRegistry.h>
#include <QFileDialog>
#include <QSplashScreen>
#include <QtCore>
#include <vector>
#include "Version.h"

namespace ai {
namespace debug {

AIDebugger::AIDebugger(int argc, char** argv) :
		QApplication(argc, argv), _running(true), _time(0L), _selectedId(-1), _socket(this) {
#ifdef Q_WS_X11
	QApplication::setGraphicsSystem(QLatin1String("raster"));
#endif
	setOrganizationDomain("simpleai");
	setApplicationName("simpleai");
	setApplicationVersion(VERSION);
#ifdef Q_WS_MAC
	a.setAttribute(Qt::AA_DontShowIconsInMenus);
#endif

	connect(&_socket, SIGNAL(readyRead()), SLOT(readTcpData()));

	ai::ProtocolHandlerRegistry::get().registerHandler(ai::PROTO_STATE, this);
}

AIDebugger::~AIDebugger() {
}

void AIDebugger::executeAIStateMessage(const AIStateMessage& msg) {
	_entities = msg.getTrees();
}

bool AIDebugger::isSelected(const ai::AIStateTree& ai) const {
	return _selectedId == ai.getId();
}

const ai::AIStateTree* AIDebugger::getSelected() const {
	if (_selectedId == -1)
		return nullptr;
	for (AIDebugger::EntitiesIter i = _entities.begin(); i != _entities.end(); ++i) {
		if (i->getId() == _selectedId)
			return &*i;
	}
	return nullptr;
}

void AIDebugger::select(const ai::AIStateTree& ai) {
	_selectedId = ai.getId();
	qDebug() << "select entity " << _selectedId;
}

void AIDebugger::unselect() {
	_selectedId = -1;
	qDebug() << "unselect entity";
}

int AIDebugger::run() {
	QSplashScreen splash(QPixmap(":/images/splash.jpg"));
	splash.show();
	splash.showMessage(tr("Loading..."), Qt::AlignLeft | Qt::AlignTop, Qt::black);
	processEvents();

	splash.close();
	AIDebuggerWindow w(*this);
	w.show();
	return exec();
}

bool AIDebugger::connectToAIServer(const QString& hostname, short port) {
	_socket.connectToHost(hostname, port);
	if (_socket.waitForConnected()) {
		return true;
	}
	return false;
}

void AIDebugger::readTcpData() {
	// TODO: this is not working if messages are split
	_socket.waitForReadyRead();
	const QByteArray& data = _socket.readAll();
	ai::streamContainer stream(data.size());
	for (int i = 0; i < data.size(); ++i) {
		stream[i] = data[i];
	}
	while (!stream.empty()) {
		ai::IProtocolMessage* msg = ai::ProtocolMessageFactory::get().create(stream);
		if (msg != nullptr) {
			ai::IProtocolHandler* handler = ai::ProtocolHandlerRegistry::get().getHandler(*msg);
			if (handler != nullptr) {
				handler->execute(1, *msg);
			} else {
				qDebug() << "no handler for " << msg->getId();
				_socket.close();
				break;
			}
		} else {
			qDebug() << "no message factory for " << (int)stream.front();
			_socket.close();
			break;
		}
	}
}

}
}
