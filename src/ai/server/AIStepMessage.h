#pragma once

#include "IProtocolMessage.h"
#include "ICharacter.h"

namespace ai {

/**
 * @brief Perform one step if the ai controlled entities are in paused mode
 *
 * Also see @c AIPauseMessage
 */
class AIStepMessage: public IProtocolMessage {
private:
	long _millis;

public:
	AIStepMessage(long millis) :
			IProtocolMessage(PROTO_STEP) {
		_millis = millis;
	}

	AIStepMessage(streamContainer& in) :
			IProtocolMessage(PROTO_STEP) {
		_millis = readLong(in);
	}

	void serialize(streamContainer& out) const override {
		addByte(out, _id);
		addLong(out, _millis);
	}

	inline long getStepMillis() const {
		return _millis;
	}
};

}
