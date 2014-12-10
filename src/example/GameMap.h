#pragma once

#include "GameEntity.h"
#include <server/Zone.h>
#include <vector>
#include <iostream>

namespace ai {
namespace example {

class GameMap: public ai::IMap {
private:
	int _size;
	ai::Zone _zone;
	ai::Server& _server;
	std::vector<ai::example::GameEntity*> _entities;

public:
	GameMap(int size, const std::string& name, ai::Server& server) :
			IMap(), _size(size), _zone(name), _server(server) {
		_server.addZone(&_zone);
	}

	~GameMap() {
		for (std::vector<ai::example::GameEntity*>::iterator i = _entities.begin(); i != _entities.end(); ++i) {
			delete *i;
		}
		_server.removeZone(&_zone);
	}

	inline const std::string& getName() const {
		return _zone.getName();
	}

	inline GameEntity* addEntity(GameEntity* entity) {
		_entities.push_back(entity);
		ai::AI& ai = *entity;
		_zone.addAI(&ai);
		return entity;
	}

	inline void update(long dt) {
		for (std::vector<ai::example::GameEntity*>::iterator i = _entities.begin(); i != _entities.end(); ++i) {
			(*i)->update(dt, _size);
		}
	}

	inline bool isBlocked(const ai::Vector3f& pos) const override {
		if (pos.x < -_size || pos.x >= _size)
			return true;
		if (pos.z < -_size || pos.z >= _size)
			return true;
		return false;
	}

	void initializeAggro() {
		// TODO: remove me once we have an attack
		for (std::vector<ai::example::GameEntity*>::iterator i = _entities.begin() + 1; i != _entities.end(); ++i) {
			ai::Entry* e = _entities[0]->addAggro(**i, 1000.0f + static_cast<float>(rand() % 1000));
			e->setReduceByValue(1.0f + static_cast<float>(rand() % 3));
		}
	}

	// returns a random start position within the boundaries
	ai::Vector3f getStartPosition() const {
		const int x = (rand() % (2 * _size)) - _size;
		const float y = 0.0f;
		const int z = (rand() % (2 * _size)) - _size;
		return ai::Vector3f(static_cast<float>(x), y, static_cast<float>(z));
	}
};

}
}
