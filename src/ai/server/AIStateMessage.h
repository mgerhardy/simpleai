#pragma once

#include "IProtocolMessage.h"
#include "AIStubTypes.h"
#include <vector>

namespace ai {

class AIStateMessage: public IProtocolMessage {
private:
	typedef std::vector<AIStateTree> Trees;
	Trees _trees;

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

	void readTree (streamContainer& in) {
		const ai::CharacterId id = readInt(in);
		const float x = readFloat(in);
		const float y = readFloat(in);
		const float z = readFloat(in);
		const ai::AIPosition position(x, y, z);
		const AIStateNode& node = readNode(in);
		const AIStateTree tree(id, position, node);
		_trees.push_back(tree);
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

	void writeTree (streamContainer& out, const AIStateTree& tree) const {
		addInt(out, tree.getId());
		const ai::AIPosition position = tree.getPosition();
		addFloat(out, position.x());
		addFloat(out, position.y());
		addFloat(out, position.z());
		const AIStateNode& node = tree.getNode();
		writeNode(out, node);
	}

public:
	AIStateMessage() :
			IProtocolMessage(PROTO_STATE) {
	}

	AIStateMessage(streamContainer& in) :
			IProtocolMessage(PROTO_STATE) {
		in.pop_front();
		const int treeSize = readInt(in);
		for (int i = 0; i < treeSize; ++i) {
			readTree(in);
		}
	}

	void addTree(const AIStateTree& tree) {
		_trees.push_back(tree);
	}

	void serialize(streamContainer& out) const override {
		addByte(out, _id);
		addInt(out, _trees.size());
		for (Trees::const_iterator i = _trees.begin(); i != _trees.end(); ++i) {
			writeTree(out, *i);
		}
	}

	inline const std::vector<AIStateTree>& getTrees() const {
		return _trees;
	}
};

}
