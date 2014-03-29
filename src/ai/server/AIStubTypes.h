#pragma once

#include <vector>
#include <AI.h>
#include <tree/TreeNode.h>

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
	std::vector<AIStateAggroEntry> _aggro;
public:
	inline void addAggro(const AIStateAggroEntry& entry) {
		_aggro.push_back(entry);
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
	TreeNodeStatus _status;
	bool _active;
public:
	AIStateNode(const std::string& name, const std::string& condition, long lastRun, TreeNodeStatus status, bool active) :
			_name(name), _condition(condition), _lastRun(lastRun), _status(status), _active(active) {
	}

	AIStateNode() :
			_lastRun(0L), _status(UNKNOWN), _active(false) {
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

	inline TreeNodeStatus getStatus() const {
		return _status;
	}

	inline bool isActive() const {
		return _active;
	}
};

class AIStateWorld {
private:
	ai::CharacterId _id;
	ai::Vector3f _position;
	float _orientation;
public:
	AIStateWorld(const ai::CharacterId& id, const ai::Vector3f& position, float orientation) :
			_id(id), _position(position), _orientation(orientation) {
	}

	inline const ai::CharacterId& getId() const {
		return _id;
	}

	inline float getOrientation() const {
		return _orientation;
	}

	inline const ai::Vector3f& getPosition() const {
		return _position;
	}
};

}
