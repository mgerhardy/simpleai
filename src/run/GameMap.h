#pragma once

#include "GameEntity.h"
#include <set>
#include <iterator>
#include <unordered_map>

namespace ai {
namespace example {

class GameMap {
private:
	int _size;
	ai::Zone _zone;
	ai::Server& _server;
	typedef std::unordered_map<CharacterId, ai::AIPtr> Entities;
	Entities _entities;
	ai::ReadWriteLock _lock;

public:
	GameMap(int size, const std::string& name, ai::Server& server) :
			_size(size), _zone(name), _server(server), _lock("gamemap") {
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
		ai::ScopedReadLock lock(_lock);
		if (_entities.empty())
			return ai::AIPtr();
		const int size = static_cast<int>(_entities.size());
		const int randomIndex = ai::random(0, size - 1);
		return std::next(_entities.begin(), randomIndex)->second;
	}

	inline const std::string& getName() const {
		return _zone.getName();
	}

	inline void addEntity(const ai::AIPtr& entity, GroupId groupId) {
		{
			ai::ScopedReadLock lock(_lock);
			_entities.insert(std::make_pair(entity->getId(), entity));
		}
		ai_assert_always(_zone.addAI(entity), "Could not add entity to zone with id %i", entity->getId());
		ai::GroupMgr& groupMgr = _zone.getGroupMgr();
		entity->getCharacter()->setAttribute(ai::attributes::GROUP, std::to_string(groupId));
		groupMgr.add(groupId, entity);
	}

	inline bool remove(const ai::CharacterId& id) {
		ai::ScopedReadLock lock(_lock);
		auto iter = _entities.find(id);
		if (iter == _entities.end())
			return false;
		_zone.removeAI(iter->second);
		_entities.erase(iter);
		return true;
	}

	inline bool remove(const ai::AIPtr& entity) {
		if (!entity)
			return false;
		return remove(entity->getId());
	}

	inline void update(int64_t dt) {
		_zone.update(dt);
	}

	inline int getSize() const {
		return _size;
	}

	void initializeAggro() {
		ai::ScopedReadLock lock(_lock);
		// TODO: remove me once we have an attack
		Entities::iterator i = _entities.begin();
		if (i == _entities.end())
			return;
		const ai::AIPtr& firstEntity = i->second;
		for (++i; i != _entities.end(); ++i) {
			ai::Entry* e = firstEntity->getAggroMgr().addAggro(i->second, 1000.0f + static_cast<float>(rand() % 1000));
			e->setReduceByValue(1.0f + static_cast<float>(rand() % 3));
		}
	}
};

}
}
