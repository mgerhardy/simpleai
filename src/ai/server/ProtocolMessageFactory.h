#pragma once

#include "Network.h"

namespace ai {

class ProtocolMessageFactory {
private:
	ProtocolMessageFactory();
public:

	static ProtocolMessageFactory& get() {
		static ProtocolMessageFactory _instance;
		return _instance;
	}

	IProtocolMessage *create(streamContainer& in);
};

}
