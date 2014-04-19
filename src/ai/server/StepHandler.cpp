#include "StepHandler.h"
#include "Server.h"

namespace ai {

void StepHandler::execute(const ClientId& /*clientId*/, const IProtocolMessage& /*message*/) {
	_server.step();
}

}
