#pragma once

#include <vector>
#include <AI.h>

namespace ai {

struct AIStateAggroEntry {
	AIStateAggroEntry(ai::CharacterId _id, float _aggro) :
			id(_id), aggro(_aggro) {
	}
	ai::CharacterId id;
	float aggro;
};

class AIStateAggro {
private:
	ai::CharacterId _id;
	std::vector<AIStateAggroEntry> _aggro;
public:
	AIStateAggro(const ai::CharacterId& id, const std::vector<AIStateAggroEntry>& aggro) :
			_id(id), _aggro(aggro) {
	}

	AIStateAggro() :
			_id(-1) {
	}

	inline void setId(const CharacterId& id) {
		_id = id;
	}

	inline void addAggro(const AIStateAggroEntry& entry) {
		_aggro.push_back(entry);
	}

	inline const ai::CharacterId& getId() const {
		return _id;
	}

	inline const std::vector<AIStateAggroEntry>& getAggro() const {
		return _aggro;
	}
};

class AIStateNode {
private:
	std::string _name;
	std::string _condition;
	typedef std::vector<AIStateNode> NodeVector;
	NodeVector _children;
	long _lastRun;
	bool _state;
public:
	AIStateNode(const std::string& name, const std::string& condition, long lastRun, bool state) :
			_name(name), _condition(condition), _lastRun(lastRun), _state(state) {
	}

	void addChildren(const AIStateNode& child) {
		_children.push_back(child);
	}

	inline const std::vector<AIStateNode>& getChildren() const {
		return _children;
	}

	inline const std::string& getName() const {
		return _name;
	}

	inline const std::string& getCondition() const {
		return _condition;
	}

	inline long getLastRun() const {
		return _lastRun;
	}

	inline bool getState() const {
		return _state;
	}
};

class AIStateTree {
private:
	AIStateNode _node;
	ai::CharacterId _id;
	ai::AIPosition _position;
public:
	AIStateTree(const ai::CharacterId& id, const ai::AIPosition& position, const AIStateNode& node) :
			_node(node), _id(id), _position(position) {
	}

	inline const AIStateNode& getNode() const {
		return _node;
	}

	inline const ai::CharacterId& getId() const {
		return _id;
	}

	inline const ai::AIPosition& getPosition() const {
		return _position;
	}
};

}
