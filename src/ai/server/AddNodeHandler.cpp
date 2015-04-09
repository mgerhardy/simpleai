#include "AddNodeHandler.h"
#include "Server.h"

namespace ai {

void AddNodeHandler::execute(const ClientId& clientId, const IProtocolMessage& message) {
	//const AIAddNodeMessage& msg = static_cast<const AIAddNodeMessage&>(message);
	(void)clientId;
	(void)message;
}

}
