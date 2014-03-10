#pragma once

#include "IProtocolHandler.h"
#include "AISelectMessage.h"

namespace ai {

class SelectHandler: public ai::IProtocolHandler {
public:
	virtual ~SelectHandler() {
	}

	void execute(const ClientId& clientId, const IProtocolMessage& message) override {
		const AISelectMessage& selectMsg = static_cast<const AISelectMessage&>(message);
		select(clientId, selectMsg.getCharacterId());
	}

	virtual void select(const ClientId& clientId, const CharacterId& id) = 0;
};

}
