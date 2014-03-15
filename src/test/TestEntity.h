#pragma once

#include <AI.h>
#include <list>

class TestEntity : public ai::ICharacter {
private:
	ai::AI _ai;

public:
	TestEntity (const ai::CharacterId& id, const ai::TreeNodePtr& root, ai::IPathfinder& pathfinder) :
			ai::ICharacter(id), _ai(*this, root, pathfinder) {
	}

	~TestEntity () {
	}

	operator ai::AI& () {
		return _ai;
	}

	void update(uint32_t deltaTime) {
		_ai.update(deltaTime);
	}
};
