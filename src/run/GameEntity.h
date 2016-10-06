#pragma once

#ifndef AI_INCLUDE_LUA
#error "simpleai-run needs lua"
#endif
#include <SimpleAI.h>
#include <list>

namespace ai {
namespace example {

class GameMap;

class GameEntity : public ai::ICharacter {
private:
	const ai::example::GameMap* _map;
	std::atomic_int _hitpoints;
	int _damage;
	int _attackDelay;

	glm::vec3 getStartPosition() const;

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

	void update (int64_t deltaTime, bool debuggingActive) override;
};

}
}
