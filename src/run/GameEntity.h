#pragma once

#include <SimpleAI.h>
#include <list>

namespace ai {
namespace example {

class GameMap;

class GameEntity : public ai::ICharacter {
private:
	const ai::example::GameMap* _map;
	GroupId _groupId;

public:
	GameEntity(const ai::CharacterId& id,
			const ai::example::GameMap* map,
			const ai::TreeNodePtr& root,
			ai::GroupMgr& groupManager) :
			ai::ICharacter(id, root, groupManager), _map(map) {
		setAttribute(ai::attributes::NAME, "Example " + std::to_string(id));
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

	void update (long deltaTime) override;
};

}
}
