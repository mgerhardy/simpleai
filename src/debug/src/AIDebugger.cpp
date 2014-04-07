#include "AIDebugger.h"
#include "AIDebuggerWidget.h"
#include "MapView.h"
#include <server/IProtocolMessage.h>
#include <server/AISelectMessage.h>
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
		QObject(), _selectedId(-1), _socket(this), _pause(false)
{
	connect(&_socket, SIGNAL(readyRead()), SLOT(readTcpData()));
	connect(&_socket, SIGNAL(disconnected()), SLOT(onDisconnect()));

	ai::ProtocolHandlerRegistry& r = ai::ProtocolHandlerRegistry::get();
	r.registerHandler(ai::PROTO_STATE, ProtocolHandlerPtr(new StateHandler(*this)));
	r.registerHandler(ai::PROTO_CHARACTER_DETAILS, ProtocolHandlerPtr(new CharacterHandler(*this)));
	r.registerHandler(ai::PROTO_PAUSE, ProtocolHandlerPtr(new PauseHandler(*this)));

	_window = new AIDebuggerWidget(*this);
}

AIDebugger::~AIDebugger() {
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

bool AIDebugger::connectToAIServer(const QString& hostname, short port) {
	_socket.connectToHost(hostname, port);
	if (_socket.waitForConnected()) {
		return true;
	}
	return false;
}

void AIDebugger::onDisconnect() {
	_pause = false;
	_selectedId = -1;
	_aggro.clear();
	_node = AIStateNode();
}

void AIDebugger::readTcpData() {
	while(_socket.bytesAvailable() > 0) {
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
}

MapView* AIDebugger::createMapWidget() {
	return new MapView(*this);
}

}
}
