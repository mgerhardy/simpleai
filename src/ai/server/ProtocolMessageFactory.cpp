#include "ProtocolMessageFactory.h"
#include "IProtocolMessage.h"
#include "AIStateMessage.h"
#include "AICharacterDetailsMessage.h"
#include "AIPauseMessage.h"
#include "AISelectMessage.h"
#include "AINamesMessage.h"
#include "AIChangeMessage.h"

namespace ai {

ProtocolMessageFactory::ProtocolMessageFactory() {
}

bool ProtocolMessageFactory::isNewMessageAvailable(const streamContainer& in) const {
	const int32_t size = IProtocolMessage::peekInt(in);
	if (size == -1) {
		// not enough data yet, wait a little bit more
		return false;
	}
	const int streamSize = static_cast<int>(in.size() - sizeof(int32_t));
	if (size > streamSize) {
		// not enough data yet, wait a little bit more
		return false;
	}
	return true;
}

IProtocolMessage *ProtocolMessageFactory::create(streamContainer& in) {
	// remove the size from the stream
	in.erase(in.begin(), std::next(in.begin(), sizeof(int32_t)));
	// get the message type
	const uint8_t type = in.front();
	in.pop_front();
	if (type == PROTO_STATE) {
		return new AIStateMessage(in);
	} else if (type == PROTO_SELECT) {
		return new AISelectMessage(in);
	} else if (type == PROTO_PAUSE) {
		return new AIPauseMessage(in);
	} else if (type == PROTO_NAMES) {
		return new AINamesMessage(in);
	} else if (type == PROTO_CHANGE) {
		return new AIChangeMessage(in);
	} else if (type == PROTO_RESET) {
		return new AIResetMessage();
	} else if (type == PROTO_STEP) {
		return new AIStepMessage();
	} else if (type == PROTO_PING) {
		return new AIPingMessage();
	} else if (type == PROTO_CHARACTER_DETAILS) {
		return new AICharacterDetailsMessage(in);
	}

	return nullptr;
}

}
