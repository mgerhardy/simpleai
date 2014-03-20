#pragma once

#include <common/Compiler.h>
#include "GameEntity.h"
#include <vector>

namespace ai {
namespace example {

class GameMap: public ai::IMap {
private:
	int _width;
	int _height;
	std::vector<ai::example::GameEntity*> _entities;

public:
	GameMap (int width, int height) :
			IMap(), _width(width), _height(height) {
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
		return entity;
	}

	inline void update (uint32_t dt) {
		for (std::vector<ai::example::GameEntity*>::iterator i = _entities.begin(); i != _entities.end(); ++i) {
			(*i)->update(dt);
		}
	}

	bool isBlocked (const ai::AIPosition& pos) const override {
		if (pos.x() < 0.0f || pos.x() >= _width)
			return true;
		if (pos.y() < 0.0f || pos.y() >= _height)
			return true;
		return false;
	}

	// returns a random start position within the boundaries
	ai::AIPosition getStartPosition() const {
		const int x = rand() % _width;
		const int y = rand() % _height;
		const float z = 0.0f;
		return ai::AIPosition(x, y, z);
	}
};

}
}
