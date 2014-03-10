#include "ProtocolMessageFactory.h"
#include "IProtocolMessage.h"
#include "AIStateMessage.h"
#include "AICharacterDetailsMessage.h"
#include "AISelectMessage.h"

namespace ai {

ProtocolMessageFactory::ProtocolMessageFactory() {
}

IProtocolMessage *ProtocolMessageFactory::create(streamContainer& in) {
	const uint8_t type = in.front();
	if (type == PROTO_STATE) {
		return new AIStateMessage(in);
	} else if (type == PROTO_SELECT) {
		return new AISelectMessage(in);
	} else if (type == PROTO_CHARACTER_DETAILS) {
		return new AICharacterDetailsMessage(in);
	}

	return nullptr;
}

}
