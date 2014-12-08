#pragma once

#include "GameEntity.h"
#include <vector>
#include <iostream>

namespace ai {
namespace example {

class GameMap: public ai::IMap {
private:
	int _size;
	std::vector<ai::example::GameEntity*> _entities;
	ai::Server _server;

public:
	GameMap (Network& network, int size, const std::string& name) :
			IMap(), _size(size), _server(network, name) {
		if (!_server.start())
			std::cerr << "Could not start the server " << name << std::endl;
	}

	~GameMap () {
		for (std::vector<ai::example::GameEntity*>::iterator i = _entities.begin(); i != _entities.end(); ++i) {
			delete *i;
		}
	}

	inline const std::string& getName () const {
		return _server.getName();
	}

	inline std::vector<ai::example::GameEntity*>& getEntities () {
		return _entities;
	}

	inline GameEntity* addEntity (GameEntity* entity) {
		_entities.push_back(entity);
		_server.addAI(*entity);
		return entity;
	}

	inline void update (long dt) {
		for (std::vector<ai::example::GameEntity*>::iterator i = _entities.begin(); i != _entities.end(); ++i) {
			(*i)->update(dt, _size);
		}
		_server.update(dt);
	}

	inline bool isBlocked (const ai::Vector3f& pos) const override {
		if (pos.x < -_size || pos.x >= _size)
			return true;
		if (pos.z < -_size || pos.z >= _size)
			return true;
		return false;
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
