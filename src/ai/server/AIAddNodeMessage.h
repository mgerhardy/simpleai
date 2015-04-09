#pragma once

#include "IProtocolMessage.h"
#include "AIStubTypes.h"

namespace ai {

/**
 * @brief Message for the remote debugging interface
 *
 * Adds a new node to some parent node
 */
class AIAddNodeMessage: public IProtocolMessage {
private:
	int32_t _parentNodeId;
	std::string _name;
	std::string _type;
	std::string _condition;

public:
	AIAddNodeMessage(int32_t parentNodeId, const std::string& name, const std::string& type, const std::string& condition) :
			IProtocolMessage(PROTO_ADDNODE), _parentNodeId(parentNodeId), _name(name), _type(type), _condition(condition) {
	}

	AIAddNodeMessage(streamContainer& in) :
			IProtocolMessage(PROTO_ADDNODE) {
		_parentNodeId = readInt(in);
		_name = readString(in);
		_type = readString(in);
		_condition = readString(in);
	}

	void serialize(streamContainer& out) const override {
		addByte(out, _id);
		addInt(out, _parentNodeId);
		addString(out, _name);
		addString(out, _type);
		addString(out, _condition);
	}

	inline const std::string& getName() const {
		return _name;
	}

	inline const std::string& getType() const {
		return _type;
	}

	inline const std::string& getCondition() const {
		return _condition;
	}

	inline uint32_t getParentNodeId() const {
		return _parentNodeId;
	}
};

}
