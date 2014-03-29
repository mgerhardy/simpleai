#pragma once

#include <SimpleAI.h>
#include <list>

namespace ai {
namespace example {

class GameEntity : public ai::ICharacter {
private:
	ai::AI _ai;
	std::list<ai::MoveVector> _route;
	float _speed;

public:
	GameEntity (const ai::CharacterId& id, const ai::TreeNodePtr& root, ai::example::Pathfinder& pathfinder, ai::GroupMgr& groupManager) :
			ai::ICharacter(id), _ai(*this, root, pathfinder, groupManager), _speed(0.1f) {
		setAttribute("Name", "Example");
	}

	~GameEntity () {
	}

	operator ai::AI& () {
		return _ai;
	}

	ai::Entry* addAggro (GameEntity& entity, float aggro) {
		return _ai.getAggroMgr().addAggro(entity, aggro);
	}

	void update (uint32_t deltaTime) {
		_ai.update(deltaTime);
		std::stringstream ss;
		ss << _position.x() << ":" << _position.y();
		setAttribute("Position", ss.str());
	}

	inline std::list<ai::MoveVector>& getRoute () {
		return _route;
	}

	inline const std::list<ai::MoveVector>& getRoute () const {
		return _route;
	}

	inline void setSpeed (float speed) {
		_speed = speed;
	}

	inline float getSpeed () const {
		return _speed;
	}
};

}
}
