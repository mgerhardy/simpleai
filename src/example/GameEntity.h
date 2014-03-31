#pragma once

#include <SimpleAI.h>
#include <list>

namespace ai {
namespace example {

class GameEntity : public ai::ICharacter {
private:
	ai::AI _ai;
	std::list<ai::MoveVector> _route;

public:
	GameEntity(const ai::CharacterId& id, const ai::TreeNodePtr& root,
			ai::example::Pathfinder& pathfinder, ai::GroupMgr& groupManager) :
			ai::ICharacter(id), _ai(*this, root, pathfinder, groupManager) {
		setAttribute("Name", "Example");
		setSpeed(id == 0 ? 50.0f : 10.0f);
	}

	~GameEntity () {
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
		if (_position.x() < -size) {
			_position.setX(size);
		} else if (_position.x() > size) {
			_position.setX(-size);
		}
		if (_position.z() < -size) {
			_position.setZ(size);
		} else if (_position.z() > size) {
			_position.setZ(-size);
		}
		_position.setY(0.0f);

		ss << _position.x() << ":" << _position.y();
		setAttribute("Id", Str::toString(getId()));
		setAttribute("Position", ss.str());
		setAttribute("Speed", Str::toString(getSpeed()));
		setAttribute("Orientation", Str::toString(toDegrees(getOrientation())));
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
