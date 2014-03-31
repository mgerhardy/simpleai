#pragma once

#include <SimpleAI.h>
#include <list>

class TestEntity : public ai::ICharacter {
private:
	ai::AI _ai;

public:
	TestEntity (const ai::CharacterId& id, const ai::TreeNodePtr& root, ai::IPathfinder& pathfinder, ai::GroupMgr& groupManager) :
			ai::ICharacter(id), _ai(*this, root, pathfinder, groupManager) {
	}

	~TestEntity () {
	}

	operator ai::AI& () {
		return _ai;
	}

	void update(long deltaTime) {
		_ai.update(deltaTime);
	}
};
