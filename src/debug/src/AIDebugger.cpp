#include "AIDebugger.h"
#include "AIDebuggerWidget.h"
#include "MapView.h"
#include <server/IProtocolMessage.h>
#include <server/AISelectMessage.h>
#include <server/AIChangeMessage.h>
#include <server/AINamesMessage.h>
#include <server/ProtocolMessageFactory.h>
#include <server/ProtocolHandlerRegistry.h>
#include <QtCore>
#include <vector>
#include "Version.h"

namespace ai {
namespace debug {

PROTOCOL_HANDLER(AIStateMessage);
PROTOCOL_HANDLER(AICharacterDetailsMessage);
PROTOCOL_HANDLER(AIPauseMessage);
PROTOCOL_HANDLER(AINamesMessage);

class StateHandler: public AIStateMessageHandler {
private:
	AIDebugger& _aiDebugger;
public:
	StateHandler (AIDebugger& aiDebugger) :
			_aiDebugger(aiDebugger) {
	}

	void executeAIStateMessage(const ai::AIStateMessage& msg) override {
		_aiDebugger.setEntities(msg.getStates());
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
		_aiDebugger.setCharacterDetails(msg.getCharacterId(), msg.getAggro(), msg.getNode(), msg.getAttributes());
	}
};

class NamesHandler: public AINamesMessageHandler {
private:
	AIDebugger& _aiDebugger;
public:
	NamesHandler (AIDebugger& aiDebugger) :
			_aiDebugger(aiDebugger) {
	}

	void executeAINamesMessage(const ai::AINamesMessage& msg) override {
		emit _aiDebugger.onNamesReceived(msg.getNames());
	}
};

class PauseHandler: public AIPauseMessageHandler {
private:
	AIDebugger& _aiDebugger;
public:
	PauseHandler (AIDebugger& aiDebugger) :
			_aiDebugger(aiDebugger) {
	}

	void executeAIPauseMessage(const ai::AIPauseMessage& msg) override {
		_aiDebugger._pause = msg.isPause();
		emit _aiDebugger.onPause(msg.isPause());
	}
};

AIDebugger::AIDebugger() :
		QObject(), _stateHandler(new StateHandler(*this)), _characterHandler(new CharacterHandler(*this)),
				_pauseHandler(new PauseHandler(*this)), _namesHandler(new NamesHandler(*this)),
				_selectedId(-1), _socket(this), _pause(false) {
	connect(&_socket, SIGNAL(readyRead()), SLOT(readTcpData()));
	connect(&_socket, SIGNAL(disconnected()), SLOT(onDisconnect()));

	ai::ProtocolHandlerRegistry& r = ai::ProtocolHandlerRegistry::get();
	r.registerHandler(ai::PROTO_STATE, _stateHandler);
	r.registerHandler(ai::PROTO_CHARACTER_DETAILS, _characterHandler);
	r.registerHandler(ai::PROTO_PAUSE, _pauseHandler);
	r.registerHandler(ai::PROTO_NAMES, _namesHandler);

	_window = new AIDebuggerWidget(*this);
}

AIDebugger::~AIDebugger() {
	delete _stateHandler;
	delete _characterHandler;
	delete _pauseHandler;
	delete _namesHandler;
}

bool AIDebugger::isSelected(const ai::AIStateWorld& ai) const {
	return _selectedId == ai.getId();
}

const CharacterId& AIDebugger::getSelected() const {
	return _selectedId;
}

void AIDebugger::togglePause() {
	const bool newPauseMode = !_pause;
	writeMessage(AIPauseMessage(newPauseMode));
}

void AIDebugger::select(const ai::AIStateWorld& ai) {
	writeMessage(AISelectMessage(ai.getId()));
}

bool AIDebugger::writeMessage(const IProtocolMessage& msg) {
	if (_socket.state() != QAbstractSocket::ConnectedState) {
		return false;
	}
	streamContainer out;
	msg.serialize(out);
	QByteArray temp;
	QDataStream data(&temp, QIODevice::ReadWrite);
	const uint32_t size = out.size();
	streamContainer sizeC;
	IProtocolMessage::addInt(sizeC, size);
	for (streamContainer::iterator i = sizeC.begin(); i != sizeC.end(); ++i) {
		const uint8_t byte = *i;
		data << byte;
	}
	for (streamContainer::iterator i = out.begin(); i != out.end(); ++i) {
		const uint8_t byte = *i;
		data << byte;
	}
	_socket.write(temp);
	return true;
}

void AIDebugger::unselect() {
	writeMessage(AISelectMessage(-1));
	_selectedId = -1;
	_aggro.clear();
	_node = AIStateNode();
	qDebug() << "unselect entity";
}

void AIDebugger::step() {
	writeMessage(AIStepMessage());
}

void AIDebugger::reset() {
	writeMessage(AIResetMessage());
}

void AIDebugger::change(const QString& name) {
	writeMessage(AIChangeMessage(name.toStdString()));
}

bool AIDebugger::connectToAIServer(const QString& hostname, short port) {
	_socket.connectToHost(hostname, port, QAbstractSocket::ReadWrite, QAbstractSocket::AnyIPProtocol);
	if (_socket.waitForConnected()) {
		return true;
	}
	const QAbstractSocket::SocketError socketError = _socket.error();
	switch (socketError) {
	case QAbstractSocket::RemoteHostClosedError:
		qDebug() << "The connection was closed by the host";
		break;
	case QAbstractSocket::HostNotFoundError:
		qDebug() << "The host was not found. Please check the host name and port settings";
		break;
	case QAbstractSocket::ConnectionRefusedError:
		qDebug() << "The connection was refused by the peer";
		break;
	default:
		qDebug() << "Socket error: " << socketError;
		break;
	}
	return false;
}

void AIDebugger::onDisconnect() {
	_pause = false;
	_selectedId = -1;
	_aggro.clear();
	_node = AIStateNode();
	emit disconnect();
}

void AIDebugger::readTcpData() {
	while (_socket.bytesAvailable() > 0) {
		const QByteArray& data = _socket.readAll();
		const int n = data.count();
		for (int i = 0; i < n; ++i) {
			_stream.push_back(data[i]);
		}
		ai::ProtocolMessageFactory& mf = ai::ProtocolMessageFactory::get();
		for (;;) {
			if (!mf.isNewMessageAvailable(_stream))
				break;
			ai::IProtocolMessage* msg = mf.create(_stream);
			if (msg == nullptr) {
				qDebug() << "unknown server message - disconnecting";
				_socket.close();
				break;
			}
			ai::ProtocolHandlerRegistry& r = ai::ProtocolHandlerRegistry::get();
			ai::IProtocolHandler* handler = r.getHandler(*msg);
			if (handler) {
				handler->execute(1, *msg);
			} else {
				qDebug() << "no handler for " << msg->getId();
				_socket.close();
				break;
			}
		}
		_window->tick();
	}
}

MapView* AIDebugger::createMapWidget() {
	return new MapView(*this);
}

}
}
