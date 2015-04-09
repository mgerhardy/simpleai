#include "DeleteNodeHandler.h"
#include "Server.h"

namespace ai {

void DeleteNodeHandler::execute(const ClientId&, const IProtocolMessage& message) {
	const AIDeleteNodeMessage& msg = static_cast<const AIDeleteNodeMessage&>(message);
	_server.deleteNode(msg.getCharacterId(), msg.getNodeId());
}

}
