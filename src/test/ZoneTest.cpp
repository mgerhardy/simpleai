#include "ZoneTest.h"

class ZoneTest: public TestSuite {
};

TEST_F(ZoneTest, testChanges) {
	ai::Zone zone("test1");
	ai::TreeNodePtr root(new ai::PrioritySelector("test", "", ai::True::get()));
	for (int i = 0; i < 100; i += 2) {
		ai::ICharacter character(i, root, _groupManager);
		ASSERT_TRUE(zone.addAI(&character.getAI())) << "Could not add ai to the zone";
		zone.setDebug(true);
		zone.update(1);
		ASSERT_TRUE(character.getAI().isDebuggingActive()) << "Debug is not active for the entity";
		ai::ICharacter character2(i + 1, root, _groupManager);
		ASSERT_TRUE(zone.addAI(&character2.getAI())) << "Could not add ai to the zone";
		zone.update(1);
		ASSERT_TRUE(character2.getAI().isDebuggingActive()) << "Debug is not active for newly added entity";
		zone.setDebug(false);
		zone.update(1);
		ASSERT_FALSE(character.getAI().isDebuggingActive()) << "Debug is still active for entity";
		ASSERT_FALSE(character2.getAI().isDebuggingActive()) << "Debug is still active for newly added entity";
		ASSERT_TRUE(zone.removeAI(&character.getAI())) << "Could not remove ai from zone";
		ASSERT_TRUE(zone.removeAI(&character2.getAI())) << "Could not remove ai from zone";
	}
}
