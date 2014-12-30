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
	ATOMICINT _hitpoints;
	int _damage;
	int _attackDelay;

public:
	GameEntity(const ai::CharacterId& id,
			const ai::example::GameMap* map,
			const ai::TreeNodePtr& root,
			ai::GroupMgr& groupManager) :
			ai::ICharacter(id, root, groupManager), _map(map), _hitpoints(100), _damage(5), _attackDelay(500) {
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
		setAttribute("Damage", std::to_string(_damage));
		setAttribute("Reloadtime", std::to_string(_attackDelay));

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

	inline int getHitpoints () const {
		return _hitpoints;
	}

	inline int getDamage () const {
		return _damage;
	}

	inline int getAttackDelay () const {
		return _attackDelay;
	}

	/**
	 * @return @c false if the dealt damage did not lead to death, @c true otherwise.
	 */
	inline bool applyDamage (int damage) {
		return (_hitpoints -= damage) <= 0;
	}

	void update (long deltaTime) override;
};

}
}
