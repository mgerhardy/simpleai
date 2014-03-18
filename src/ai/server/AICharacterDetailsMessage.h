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
	const CharacterAttributes* _attributesPtr;
	AIStateAggro _aggro;
	AIStateNode _root;
	CharacterAttributes _attributes;

	AIStateNode readNode (streamContainer& in) {
		const std::string& name = readString(in);
		const std::string& condition = readString(in);
		const int64_t lastRun = readLong(in);
		const bool state = readBool(in);
		const uint8_t childrenCount = readShort(in);
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
		addShort(out, children.size());
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

	void writeAttributes(streamContainer& out, const CharacterAttributes& a) const {
		addShort(out, a.size());
		for (CharacterAttributes::const_iterator i = a.begin(); i != a.end(); ++i) {
			addString(out, i->first);
			addString(out, i->second);
		}
	}

	void readAttributes(streamContainer& in, CharacterAttributes& attributes) const {
		const int size = readShort(in);
		for (int i = 0; i < size; ++i) {
			const std::string& key = readString(in);
			const std::string& value = readString(in);
			attributes.insert(std::make_pair(key, value));
		}
	}

public:
	AICharacterDetailsMessage(const CharacterId& id, const AIStateAggro& aggro, const AIStateNode& root, const CharacterAttributes& attributes) :
			IProtocolMessage(PROTO_CHARACTER_DETAILS), _chrId(id), _aggroPtr(&aggro), _rootPtr(&root), _attributesPtr(&attributes) {
	}

	AICharacterDetailsMessage(streamContainer& in) :
			IProtocolMessage(PROTO_CHARACTER_DETAILS), _aggroPtr(nullptr), _rootPtr(nullptr), _attributesPtr(nullptr) {
		_chrId = readInt(in);
		readAggro(in, _aggro);
		_root = readNode(in);
		readAttributes(in, _attributes);
	}

	void serialize(streamContainer& out) const override {
		addByte(out, _id);
		addInt(out, _chrId);
		writeAggro(out, *_aggroPtr);
		writeNode(out, *_rootPtr);
		writeAttributes(out, *_attributesPtr);
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

	inline const CharacterAttributes& getAttributes () const {
		return _attributes;
	}
};

}
