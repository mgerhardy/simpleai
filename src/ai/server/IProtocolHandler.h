#pragma once

#include <common/Compiler.h>
#include <common/Pointers.h>
#include "IProtocolMessage.h"

namespace ai {

typedef uint8_t ClientId;

/**
 * @brief Interface for the execution of assigned IProtocolMessage
 *
 * @note Register handler implementations at the @c ProtocolHandlerRegistry
 */
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

typedef SharedPtr<IProtocolHandler> ProtocolHandlerPtr;

/**
 * @brief Use this deleter for any handler that should not get freed by @c delete.
 */
struct ProtocolHandlerNopDeleter {
	void operator()(IProtocolHandler* /* ptr */) {
	}
};

}
