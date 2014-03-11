#include "AIDebugger.h"
#include "AIDebuggerWindow.h"
#include "MapView.h"
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

PROTOCOL_HANDLER(AIStateMessage);
PROTOCOL_HANDLER(AICharacterDetailsMessage);

class StateHandler: public AIStateMessageHandler {
private:
	AIDebugger& _aiDebugger;
public:
	StateHandler (AIDebugger& aiDebugger) :
			_aiDebugger(aiDebugger) {
	}

	void executeAIStateMessage(const ai::AIStateMessage& msg) override {
		_aiDebugger.setEntities(msg.getTrees());
	}
};

class CharacterHandler: public AICharacterDetailsMessageHandler {
private:
	AIDebugger& _aiDebugger;
public:
	CharacterHandler (AIDebugger& aiDebugger) :
			_aiDebugger(aiDebugger) {
	}

	void executeAICharacterDetailsMessage(const ai::AICharacterDetailsMessage& msg) override {
		_aiDebugger.setCharacterDetails(msg.getCharacterId(), msg.getAggro());
	}
};

AIDebugger::AIDebugger(int argc, char** argv) :
		QApplication(argc, argv), _running(true), _time(0L), _selectedId(-1), _socket(this)
{
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

	ai::ProtocolHandlerRegistry& r = ai::ProtocolHandlerRegistry::get();
	r.registerHandler(ai::PROTO_STATE, ProtocolHandlerPtr(new StateHandler(*this)));
	r.registerHandler(ai::PROTO_CHARACTER_DETAILS, ProtocolHandlerPtr(new CharacterHandler(*this)));
}

AIDebugger::~AIDebugger() {
}

bool AIDebugger::isSelected(const ai::AIStateTree& ai) const {
	return _selectedId == ai.getId();
}

const ai::AIStateTree* AIDebugger::getSelected() {
	if (_selectedId == -1)
		return nullptr;
	for (AIDebugger::EntitiesIter i = _entities.begin(); i != _entities.end(); ++i) {
		if (i->getId() == _selectedId)
			return &*i;
	}
	unselect();
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
	_socket.waitForReadyRead();
	const QByteArray& data = _socket.readAll();
	for (int i = 0; i < data.size(); ++i) {
		_stream.push_back(data[i]);
	}
	qDebug() << "read " << data.size() << " bytes";
	ai::ProtocolMessageFactory& mf = ai::ProtocolMessageFactory::get();
	for (;;) {
		ai::IProtocolMessage* msg = mf.create(_stream);
		if (msg == nullptr)
			break;
		qDebug() << "got message";
		ai::ProtocolHandlerRegistry& r = ai::ProtocolHandlerRegistry::get();
		ai::ProtocolHandlerPtr handler = r.getHandler(*msg);
		if (handler) {
			handler->execute(1, *msg);
		} else {
			qDebug() << "no handler for " << msg->getId();
			_socket.close();
			break;
		}
	}
}

MapView* AIDebugger::createMapWidget() {
	return new MapView(*this);
}

}
}
