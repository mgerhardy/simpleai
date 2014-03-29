#pragma once

#include "GameEntity.h"
#include <vector>
#include <iostream>

namespace ai {
namespace example {

#define PORT 12345

class GameMap: public ai::IMap {
private:
	int _width;
	int _height;
	std::vector<ai::example::GameEntity*> _entities;
	ai::Server _server;

public:
	GameMap (int width, int height) :
			IMap(), _width(width), _height(height), _server(PORT) {
		if (!_server.start())
			std::cerr << "Could not start the server" << std::endl;
		else
			std::cout << "Started the server and accept connections on port " << PORT << std::endl;
	}

	~GameMap () {
		for (std::vector<ai::example::GameEntity*>::iterator i = _entities.begin(); i != _entities.end(); ++i) {
			delete *i;
		}
	}

	inline std::vector<ai::example::GameEntity*>& getEntities () {
		return _entities;
	}

	inline GameEntity* addEntity (GameEntity* entity) {
		_entities.push_back(entity);
		_server.addAI(*entity);
		return entity;
	}

	inline void update (uint32_t dt) {
		for (std::vector<ai::example::GameEntity*>::iterator i = _entities.begin(); i != _entities.end(); ++i) {
			(*i)->update(dt);
		}
		_server.update(dt);
	}

	bool isBlocked (const ai::Vector3f& pos) const override {
		if (pos.x() < 0.0f || pos.x() >= _width)
			return true;
		if (pos.y() < 0.0f || pos.y() >= _height)
			return true;
		return false;
	}

	// returns a random start position within the boundaries
	ai::Vector3f getStartPosition() const {
		const int x = rand() % _width;
		const int y = rand() % _height;
		const float z = 0.0f;
		return ai::Vector3f(x, y, z);
	}
};

}
}
