#pragma once

#include "IProtocolMessage.h"

namespace ai {

class IProtocolHandler {
public:
	virtual ~IProtocolHandler() {
	}

	virtual void execute(const ClientId& clientId, const IProtocolMessage& message) = 0;
};

}
