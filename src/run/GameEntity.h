#pragma once

#include <SimpleAI.h>
#include <list>

namespace ai {
namespace example {

class GameMap;

class GameEntity : public ai::ICharacter {
private:
	const ai::example::GameMap* _map;
	ATOMICINT _hitpoints;
	int _damage;
	int _attackDelay;

	ai::Vector3f getStartPosition() const;

public:
	GameEntity(const ai::CharacterId& id, const ai::example::GameMap* map);

	~GameEntity () {
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

	void update (long deltaTime, bool debuggingActive) override;
};

}
}
