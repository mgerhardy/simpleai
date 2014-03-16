#pragma once

#include "IProtocolMessage.h"
#include "AIStubTypes.h"
#include <vector>

namespace ai {

class AIStateMessage: public IProtocolMessage {
private:
	typedef std::vector<AIStateWorld> States;
	States _states;

	void readState (streamContainer& in) {
		const ai::CharacterId id = readInt(in);
		const float x = readFloat(in);
		const float y = readFloat(in);
		const float z = readFloat(in);
		const ai::AIPosition position(x, y, z);
		const AIStateWorld tree(id, position);
		_states.push_back(tree);
	}

	void writeState (streamContainer& out, const AIStateWorld& state) const {
		addInt(out, state.getId());
		const ai::AIPosition& position = state.getPosition();
		addFloat(out, position.x());
		addFloat(out, position.y());
		addFloat(out, position.z());
	}

public:
	AIStateMessage() :
			IProtocolMessage(PROTO_STATE) {
	}

	AIStateMessage(streamContainer& in) :
			IProtocolMessage(PROTO_STATE) {
		const int treeSize = readInt(in);
		for (int i = 0; i < treeSize; ++i) {
			readState(in);
		}
	}

	void addState(const AIStateWorld& tree) {
		_states.push_back(tree);
	}

	void serialize(streamContainer& out) const override {
		addByte(out, _id);
		addInt(out, _states.size());
		for (States::const_iterator i = _states.begin(); i != _states.end(); ++i) {
			writeState(out, *i);
		}
	}

	inline const std::vector<AIStateWorld>& getStates() const {
		return _states;
	}
};

}
