#include "DeleteNodeHandler.h"
#include "Server.h"

namespace ai {

void DeleteNodeHandler::execute(const ClientId& clientId, const IProtocolMessage& message) {
	//const AIDeleteNodeMessage& msg = static_cast<const AIDeleteNodeMessage&>(message);
	(void)clientId;
	(void)message;
}

}
