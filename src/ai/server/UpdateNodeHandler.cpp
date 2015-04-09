#include "UpdateNodeHandler.h"
#include "Server.h"

namespace ai {

void UpdateNodeHandler::execute(const ClientId& clientId, const IProtocolMessage& message) {
	//const AIUpdateNodeMessage& msg = static_cast<const AIUpdateNodeMessage&>(message);
	(void)clientId;
	(void)message;
}

}
