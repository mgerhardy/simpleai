#pragma once

#include "IProtocolMessage.h"
#include "AIStubTypes.h"
#include <vector>

namespace ai {

class AICharacterDetailsMessage: public IProtocolMessage {
private:
	CharacterId _chrId;
	const AIStateAggro* _aggroPtr;
	const AIStateNode* _rootPtr;
	AIStateAggro _aggro;
	AIStateNode _root;

	AIStateNode readNode (streamContainer& in) {
		const std::string& name = readString(in);
		const std::string& condition = readString(in);
		const int64_t lastRun = readLong(in);
		const bool state = readBool(in);
		const uint8_t childrenCount = readByte(in);
		AIStateNode node(name, condition, lastRun, state);
		for (uint8_t i = 0; i < childrenCount; ++i) {
			const AIStateNode& child = readNode(in);
			node.addChildren(child);
		}
		return node;
	}

	void writeNode (streamContainer& out, const AIStateNode& node) const {
		addString(out, node.getName());
		addString(out, node.getCondition());
		addLong(out, node.getLastRun());
		addBool(out, node.getState());
		const std::vector<AIStateNode>& children = node.getChildren();
		addByte(out, children.size());
		for (std::vector<AIStateNode>::const_iterator i = children.begin(); i != children.end(); ++i) {
			writeNode(out, *i);
		}
	}

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
	AICharacterDetailsMessage(const CharacterId& id, const AIStateAggro& aggro, const AIStateNode& root) :
			IProtocolMessage(PROTO_CHARACTER_DETAILS), _chrId(id), _aggroPtr(&aggro), _rootPtr(&root) {
	}

	AICharacterDetailsMessage(streamContainer& in) :
			IProtocolMessage(PROTO_CHARACTER_DETAILS), _aggroPtr(nullptr), _rootPtr(nullptr) {
		_chrId = readInt(in);
		readAggro(in, _aggro);
		_root = readNode(in);
	}

	void serialize(streamContainer& out) const override {
		addByte(out, _id);
		addInt(out, _chrId);
		writeAggro(out, *_aggroPtr);
		writeNode(out, *_rootPtr);
	}

	inline const CharacterId& getCharacterId() const {
		return _chrId;
	}

	inline const AIStateAggro& getAggro() const {
		return _aggro;
	}

	inline const AIStateNode& getNode() const {
		return _root;
	}
};

}
