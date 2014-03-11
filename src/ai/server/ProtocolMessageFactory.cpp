#include "ProtocolMessageFactory.h"
#include "IProtocolMessage.h"
#include "AIStateMessage.h"
#include "AICharacterDetailsMessage.h"
#include "AISelectMessage.h"

namespace ai {

ProtocolMessageFactory::ProtocolMessageFactory() {
}

IProtocolMessage *ProtocolMessageFactory::create(streamContainer& in) {
	const int32_t size = IProtocolMessage::peekInt(in);
	if (size == -1) {
		return nullptr;
	}
	const int streamSize = in.size();
	if (size > streamSize) {
		// not enough data yet, wait a little bit more
		return nullptr;
	}
	// remove the size from the stream
	in.erase(in.begin(), in.begin() + 4);
	// get the message type
	const uint8_t type = in.front();
	in.pop_front();
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
