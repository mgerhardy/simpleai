#pragma once

#include "IProtocolMessage.h"

namespace ai {

class AISelectMessage: public IProtocolMessage {
private:
	ai::CharacterId _chrId;

public:
	AISelectMessage(ai::CharacterId id) :
			IProtocolMessage(PROTO_SELECT) {
		_chrId = id;
	}

	AISelectMessage(streamContainer& in) :
			IProtocolMessage(PROTO_SELECT) {
		_chrId = readInt(in);
	}

	void serialize(streamContainer& out) const override {
		addByte(out, _id);
		addInt(out, _chrId);
	}

	inline const CharacterId& getCharacterId() const {
		return _chrId;
	}
};

}
