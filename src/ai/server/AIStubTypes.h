#pragma once

#include <vector>
#include <AI.h>
#include <tree/TreeNode.h>

namespace ai {

/**
 * @brief The aggro entry for the @c AIStateAggro
 *
 * Holds a character id and the assigned aggro value
 */
struct AIStateAggroEntry {
	AIStateAggroEntry(ai::CharacterId _id, float _aggro) :
			id(_id), aggro(_aggro) {
	}
	ai::CharacterId id;
	float aggro;
};

/**
 * @brief The list of aggro entry for a character
 */
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

/**
 * @brief This is a representation of a behaviour tree node for the serialization
 */
class AIStateNode {
private:
	std::string _name;
	std::string _condition;
	typedef std::vector<AIStateNode> NodeVector;
	NodeVector _children;
	int64_t _lastRun;
	TreeNodeStatus _status;
	bool _active;
public:
	AIStateNode(const std::string& name, const std::string& condition, int64_t lastRun, TreeNodeStatus status, bool active) :
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

	/**
	 * @return The milliseconds of the last execution of this particular node
	 */
	inline int64_t getLastRun() const {
		return _lastRun;
	}

	/**
	 * @return The @c TreeNodeStatus of the last execution
	 */
	inline TreeNodeStatus getStatus() const {
		return _status;
	}

	/**
	 * @brief Some nodes have a state that holds which children was the last active one.
	 * @return Whether this particular node is active
	 */
	inline bool isActive() const {
		return _active;
	}
};

/**
 * @brief This is a representation of a character state for the serialization
 */
class AIStateWorld {
private:
	ai::CharacterId _id;
	ai::Vector3f _position;
	float _orientation;
public:
	AIStateWorld(const ai::CharacterId& id, const ai::Vector3f& position, float orientation = -1.0f) :
			_id(id), _position(position), _orientation(orientation) {
	}

	/**
	 * @return The unique id that can be used to identify the character in the world
	 */
	inline const ai::CharacterId& getId() const {
		return _id;
	}

	/**
	 * @return The orientation of the character [0, 2*PI]
	 *
	 * @note A negative value means, that the character does not have any orientation
	 */
	inline float getOrientation() const {
		return _orientation;
	}

	/**
	 * @return The position in the world
	 */
	inline const ai::Vector3f& getPosition() const {
		return _position;
	}
};

}
