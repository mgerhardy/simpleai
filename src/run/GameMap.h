#pragma once

#include "GameEntity.h"
#include <set>
#include <iterator>
#include <iostream>

namespace ai {
namespace example {

class GameMap {
private:
	int _size;
	ai::Zone _zone;
	ai::Server& _server;
	typedef std::set<ai::example::GameEntity*> Entities;
	Entities _entities;
	MUTEX(_mutex);

public:
	GameMap(int size, const std::string& name, ai::Server& server) :
			_size(size), _zone(name), _server(server) {
		_server.addZone(&_zone);
	}

	~GameMap() {
		for (Entities::iterator i = _entities.begin(); i != _entities.end(); ++i) {
			delete *i;
		}
		_server.removeZone(&_zone);
	}

	inline const ai::Zone& getZone() const {
		return _zone;
	}

	inline ai::Zone& getZone() {
		return _zone;
	}

	inline GameEntity* getRandomEntity() const {
		SCOPEDLOCK(_mutex);
		if (_entities.empty())
			return nullptr;
		const int size = static_cast<int>(_entities.size());
		const int randomIndex = ai::random(0, size - 1);
		Entities::iterator i = _entities.begin();
		std::advance(i, randomIndex);
		return *i;
	}

	inline const std::string& getName() const {
		return _zone.getName();
	}

	inline GameEntity* addEntity(GameEntity* entity) {
		{
			SCOPEDLOCK(_mutex);
			_entities.insert(entity);
		}
		ai_assert(_zone.addAI(&entity->getAI()), "Could not add entity to zone");
		entity->onAdd();
		return entity;
	}

	inline bool remove(const ai::CharacterId& id) {
		SCOPEDLOCK(_mutex);
		// TODO: improve
		for (Entities::iterator i = _entities.begin(); i != _entities.end(); ++i) {
			GameEntity* entity = *i;
			if (entity->getId() == id) {
				if (!remove(entity))
					return false;
				delete entity;
				return true;
			}
		}
		return false;
	}

	inline bool remove(GameEntity* entity) {
		if (entity == nullptr)
			return false;
		{
			SCOPEDLOCK(_mutex);
			if (_entities.erase(entity) != 1)
				return false;
		}
		ai::AI& ai = *entity;
		_zone.removeAI(&ai);
		return true;
	}

	inline void update(long dt) {
		_zone.update(dt);
	}

	inline int getSize() const {
		return _size;
	}

	void initializeAggro() {
		SCOPEDLOCK(_mutex);
		// TODO: remove me once we have an attack
		Entities::iterator i = _entities.begin();
		if (i == _entities.end())
			return;
		GameEntity *firstEntity = *i++;
		for (; i != _entities.end(); ++i) {
			ai::Entry* e = firstEntity->addAggro(**i, 1000.0f + static_cast<float>(rand() % 1000));
			e->setReduceByValue(1.0f + static_cast<float>(rand() % 3));
		}
	}
};

}
}
