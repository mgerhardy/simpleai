#pragma once

#include <SimpleAI.h>
#include <list>

class TestEntity : public ai::ICharacter {
public:
	TestEntity (const ai::CharacterId& id, const ai::TreeNodePtr& root, ai::GroupMgr& groupManager) :
			ai::ICharacter(id, root, groupManager) {
	}
};
