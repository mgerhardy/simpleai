#pragma once

#include <SimpleAI.h>
#include <list>

namespace ai {
namespace example {

class GameEntity : public ai::ICharacter {
private:
	ai::AI _ai;
	std::list<ai::MoveVector> _route;
	GroupId _groupId;

public:
	GameEntity(const ai::CharacterId& id, const ai::TreeNodePtr& root,
			ai::example::Pathfinder& pathfinder, ai::GroupMgr& groupManager) :
			ai::ICharacter(id), _ai(*this, root, pathfinder, groupManager) {
		setAttribute(ai::attributes::NAME, "Example");
		setSpeed(50.0f);
		if (id == 0)
			_groupId = 1;
		else if (id < 10)
			_groupId = 2;
		else
			_groupId = 3;

		setAttribute(ai::attributes::GROUP, std::to_string(_groupId));
		setAttribute(ai::attributes::ID, std::to_string(getId()));

		_ai.getGroupMgr().add(_groupId, this);
	}

	~GameEntity () {
		_ai.getGroupMgr().remove(_groupId, this);
	}

	operator ai::AI& () {
		return _ai;
	}

	ai::Entry* addAggro (GameEntity& entity, float aggro) {
		return _ai.getAggroMgr().addAggro(entity, aggro);
	}

	void update (long deltaTime, int size) {
		_ai.update(deltaTime);
		std::stringstream ss;
		const float sizeF = static_cast<float>(size);
		if (_position.x < -sizeF) {
			_position.x = sizeF;
		} else if (_position.x > sizeF) {
			_position.x = -sizeF;
		}
		if (_position.z < -sizeF) {
			_position.z = sizeF;
		} else if (_position.z > sizeF) {
			_position.z = -sizeF;
		}
		_position.y = 0.0f;
		// TODO: switch direction when the respawn on another side of the map

		ss << _position;
		setAttribute("Position", ss.str());
		setAttribute("Speed", std::to_string(getSpeed()));
		setAttribute("Orientation", std::to_string(toDegrees(getOrientation())));
	}

	inline std::list<ai::MoveVector>& getRoute () {
		return _route;
	}

	inline const std::list<ai::MoveVector>& getRoute () const {
		return _route;
	}
};

}
}
