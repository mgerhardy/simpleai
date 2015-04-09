#pragma once

#include "IProtocolMessage.h"
#include "AIStubTypes.h"

namespace ai {

/**
 * @brief Message for the remote debugging interface
 *
 * Deletes a node
 */
class AIDeleteNodeMessage: public IProtocolMessage {
private:
	int32_t _nodeId;

public:
	AIDeleteNodeMessage(int32_t nodeId) :
			IProtocolMessage(PROTO_DELETENODE), _nodeId(nodeId) {
	}

	AIDeleteNodeMessage(streamContainer& in) :
			IProtocolMessage(PROTO_DELETENODE) {
		_nodeId = readInt(in);
	}

	void serialize(streamContainer& out) const override {
		addByte(out, _id);
		addInt(out, _nodeId);
	}

	inline uint32_t getNodeId() const {
		return _nodeId;
	}
};

}
