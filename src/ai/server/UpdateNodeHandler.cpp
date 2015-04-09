#include "UpdateNodeHandler.h"
#include "Server.h"

namespace ai {

void UpdateNodeHandler::execute(const ClientId&, const IProtocolMessage& message) {
	const AIUpdateNodeMessage& msg = static_cast<const AIUpdateNodeMessage&>(message);
	_server.updateNode(msg.getCharacterId(), msg.getNodeId(), msg.getName(), msg.getType(), msg.getCondition());
}

}
