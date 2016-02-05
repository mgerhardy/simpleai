#pragma once

#include "GameEntity.h"
#include <set>
#include <iterator>
#include <iostream>
#include <unordered_set>

namespace ai {
namespace example {

class GameMap {
private:
	int _size;
	ai::Zone _zone;
	ai::Server& _server;
	typedef std::unordered_set<ai::AIPtr> Entities;
	Entities _entities;
	MUTEX(_mutex);

public:
	GameMap(int size, const std::string& name, ai::Server& server) :
			_size(size), _zone(name), _server(server) {
		_server.addZone(&_zone);
	}

	~GameMap() {
		_entities.clear();
		_server.removeZone(&_zone);
	}

	inline const ai::Zone& getZone() const {
		return _zone;
	}

	inline ai::Zone& getZone() {
		return _zone;
	}

	inline ai::AIPtr getRandomEntity() const {
		SCOPEDLOCK(_mutex);
		if (_entities.empty())
			return ai::AIPtr();
		const int size = static_cast<int>(_entities.size());
		const int randomIndex = ai::random(0, size - 1);
		Entities::const_iterator i = _entities.begin();
		std::advance(i, randomIndex);
		return *i;
	}

	inline const std::string& getName() const {
		return _zone.getName();
	}

	inline ai::AIPtr addEntity(const ai::AIPtr& entity, GroupId groupId) {
		{
			SCOPEDLOCK(_mutex);
			_entities.insert(entity);
		}
		ai_assert(_zone.addAI(entity), "Could not add entity to zone with id " + std::to_string(entity->getId()));
		ai::GroupMgr& groupMgr = _zone.getGroupMgr();
		entity->getCharacter()->setAttribute(ai::attributes::GROUP, std::to_string(groupId));
		groupMgr.add(groupId, entity);
		return entity;
	}

	inline bool remove(const ai::CharacterId& id) {
		SCOPEDLOCK(_mutex);
		// TODO: improve
		for (Entities::iterator i = _entities.begin(); i != _entities.end(); ++i) {
			const ai::AIPtr& entity = *i;
			if (entity->getId() == id) {
				if (!remove(entity))
					return false;
				return true;
			}
		}
		return false;
	}

	inline bool remove(const ai::AIPtr& entity) {
		if (!entity)
			return false;
		{
			SCOPEDLOCK(_mutex);
			if (_entities.erase(entity) != 1)
				return false;
		}
		_zone.removeAI(entity);
		return true;
	}

	inline void update(int64_t dt) {
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
		const ai::AIPtr& firstEntity = *i++;
		for (; i != _entities.end(); ++i) {
			ai::Entry* e = firstEntity->getAggroMgr().addAggro((*i), 1000.0f + static_cast<float>(rand() % 1000));
			e->setReduceByValue(1.0f + static_cast<float>(rand() % 3));
		}
	}
};

}
}
