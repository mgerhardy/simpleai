#pragma once

#include <common/Compiler.h>
#include "IProtocolMessage.h"

namespace ai {

class IProtocolHandler {
public:
	virtual ~IProtocolHandler() {
	}

	virtual void execute(const ClientId& clientId, const IProtocolMessage& message) = 0;
};

#define PROTOCOL_HANDLER(MessageClass) \
class MessageClass##Handler: public IProtocolHandler { \
public: \
	void execute(const ClientId& /*clientId*/, const IProtocolMessage& message) override { \
		const MessageClass& msg = static_cast<const MessageClass&>(message); \
		execute##MessageClass(msg); \
	} \
	virtual void execute##MessageClass(const MessageClass& msg) = 0; \
}

}
