#pragma once

#include "IProtocolMessage.h"

namespace ai {

class AIPauseMessage: public IProtocolMessage {
private:
	bool _pause;

public:
	AIPauseMessage(bool pause) :
			IProtocolMessage(PROTO_PAUSE) {
		_pause = pause;
	}

	AIPauseMessage(streamContainer& in) :
			IProtocolMessage(PROTO_PAUSE) {
		_pause = readBool(in);
	}

	void serialize(streamContainer& out) const override {
		addByte(out, _id);
		addBool(out, _pause);
	}

	inline bool isPause() const {
		return _pause;
	}
};

}
