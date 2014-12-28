#pragma once

#include <SimpleAI.h>
#include <list>

namespace ai {
namespace example {

class GameEntity : public ai::ICharacter {
private:
	ai::AI _ai;
	GroupId _groupId;

public:
	GameEntity(const ai::CharacterId& id, const ai::TreeNodePtr& root,
			ai::GroupMgr& groupManager) :
			ai::ICharacter(id), _ai(*this, root, groupManager) {
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

	void update (long deltaTime, int size) {
		_ai.update(deltaTime);
		std::stringstream ss;
		const float sizeF = static_cast<float>(size);
		Vector3f newPos(0.0f, 0.0f, 0.0f);
		Vector3f currentPos = _position;
		if (currentPos.x < -sizeF) {
			newPos.x = sizeF;
		} else if (currentPos.x > sizeF) {
			newPos.x = -sizeF;
		}
		if (currentPos.z < -sizeF) {
			newPos.z = sizeF;
		} else if (currentPos.z > sizeF) {
			newPos.z = -sizeF;
		}
		setPosition(newPos);
		// TODO: switch direction when the respawn on another side of the map

		ss << _position;
		setAttribute(ai::attributes::POSITION, ss.str());
		setAttribute(ai::attributes::SPEED, std::to_string(getSpeed()));
		setAttribute(ai::attributes::ORIENTATION, std::to_string(toDegrees(getOrientation())));
	}
};

}
}
