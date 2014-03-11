#pragma once

#include "IProtocolMessage.h"
#include "AIStubTypes.h"
#include <vector>

namespace ai {

class AICharacterDetailsMessage: public IProtocolMessage {
private:
	CharacterId _chrId;
	AIStateAggro _aggro;

	void writeAggro(streamContainer& out, const AIStateAggro& aggro) const {
		const std::vector<AIStateAggroEntry>& a = aggro.getAggro();
		addShort(out, a.size());
		for (std::vector<AIStateAggroEntry>::const_iterator i = a.begin(); i != a.end(); ++i) {
			addInt(out, i->id);
			addFloat(out, i->aggro);
		}
	}

	void readAggro(streamContainer& in, AIStateAggro& aggro) const {
		const int size = readShort(in);
		for (int i = 0; i < size; ++i) {
			const CharacterId chrId = readInt(in);
			const float aggroVal = readFloat(in);
			aggro.addAggro(AIStateAggroEntry(chrId, aggroVal));
		}
	}

public:
	AICharacterDetailsMessage(const CharacterId& id, const AIStateAggro& aggro) :
			IProtocolMessage(PROTO_CHARACTER_DETAILS), _chrId(id), _aggro(aggro) {
	}

	AICharacterDetailsMessage(streamContainer& in) :
			IProtocolMessage(PROTO_CHARACTER_DETAILS) {
		_chrId = readInt(in);
		readAggro(in, _aggro);
		_aggro.setId(_chrId);
	}

	void serialize(streamContainer& out) const override {
		addByte(out, _id);
		addInt(out, _chrId);
		writeAggro(out, _aggro);
	}

	inline const CharacterId& getCharacterId() const {
		return _chrId;
	}

	inline const AIStateAggro& getAggro() const {
		return _aggro;
	}
};

}
