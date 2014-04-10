#pragma once

#include "IProtocolMessage.h"
#include "ICharacter.h"

namespace ai {

/**
 * @brief Message for the remote debugging interface
 *
 * Allows to select a particular subset of entities to receive the state for
 */
class AIChangeMessage: public IProtocolMessage {
private:
	std::string _name;

public:
	AIChangeMessage(const std::string& name) :
			IProtocolMessage(PROTO_CHANGE) {
		_name = name;
	}

	AIChangeMessage(streamContainer& in) :
			IProtocolMessage(PROTO_CHANGE) {
		_name = readString(in);
	}

	void serialize(streamContainer& out) const override {
		addByte(out, _id);
		addString(out, _name);
	}

	inline const std::string& getName() const {
		return _name;
	}
};

}
