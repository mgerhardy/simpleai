#include "PauseHandler.h"
#include "Server.h"
#include <iostream>

namespace ai {

void PauseHandler::execute(const ClientId& clientId, const IProtocolMessage& message) override {
	const AIPauseMessage& msg = static_cast<const AIPauseMessage&>(message);
	_server.pause(clientId, msg.isPause());
	std::cout << "PAUSE" << std::endl;
}

}
