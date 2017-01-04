#include "ZoneTest.h"

class ZoneTest: public TestSuite {
protected:
	static std::vector<ai::AIPtr> _v;

	static void SetUpTestCase() {
		ai::TreeNodePtr root = std::make_shared<ai::PrioritySelector>("test", "", ai::True::get());
		const int n = 100000;
		_v.reserve(n);
		for (int i = 0; i < n; ++i) {
			ai::ICharacterPtr character = std::make_shared<TestEntity>(i);
			ai::AIPtr ai = std::make_shared<ai::AI>(root);
			ai->setPause(true);
			ai->setCharacter(character);
			_v.push_back(ai);
		}
	}

	static void TearDownTestCase() {
		_v.clear();
	}

	void massAdd(int n) {
		ai::Zone zone("test1");
		std::vector<ai::AIPtr> v(n);
		ASSERT_TRUE(n <= _v.size());
		for (int i = 0; i < n; ++i) {
			v.push_back(_v[i]);
		}
		ASSERT_TRUE(zone.addAIs(v)) << "Could not add ai to the zone";
		zone.update(0l);
		ASSERT_EQ(n, (int)zone.size());
	}

	void singleAdd(int n) {
		ai::Zone zone("test1");
		ASSERT_TRUE(n <= _v.size());
		for (int i = 0; i < n; ++i) {
			ASSERT_TRUE(zone.addAI(_v[i])) << "Could not add ai to the zone";
		}
		zone.update(0l);
		ASSERT_EQ(n, (int)zone.size());
	}
};

std::vector<ai::AIPtr> ZoneTest::_v;

TEST_F(ZoneTest, testChanges) {
	ai::Zone zone("test1");
	ai::TreeNodePtr root = std::make_shared<ai::PrioritySelector>("test", "", ai::True::get());
	ai::ICharacterPtr character = std::make_shared<TestEntity>(1);
	ai::AIPtr ai = std::make_shared<ai::AI>(root);
	ai->setCharacter(character);

	ai::ICharacterPtr character2 = std::make_shared<TestEntity>(2);
	ai::AIPtr ai2 = std::make_shared<ai::AI>(root);
	ai2->setCharacter(character2);

	ASSERT_TRUE(zone.addAI(ai)) << "Could not add ai to the zone";
	zone.setDebug(true);
	zone.update(1);
	ASSERT_TRUE(ai->isDebuggingActive()) << "Debug is not active for the entity";
	ASSERT_TRUE(zone.addAI(ai2)) << "Could not add ai to the zone";
	zone.update(1);
	ASSERT_TRUE(ai2->isDebuggingActive()) << "Debug is not active for newly added entity";
	zone.setDebug(false);
	zone.update(1);
	ASSERT_EQ(2u, zone.size());
	ASSERT_FALSE(ai->isDebuggingActive()) << "Debug is still active for entity";
	ASSERT_FALSE(ai2->isDebuggingActive()) << "Debug is still active for newly added entity";
	ASSERT_TRUE(zone.removeAI(ai)) << "Could not remove ai from zone";
	ASSERT_TRUE(zone.removeAI(ai2)) << "Could not remove ai from zone";
	zone.update(1);
	ASSERT_EQ(0u, zone.size());
}

TEST_F(ZoneTest, testMassAdd10) {
	massAdd(10);
}

TEST_F(ZoneTest, testMassAdd100) {
	massAdd(100);
}

TEST_F(ZoneTest, testMassAdd1000) {
	massAdd(1000);
}

TEST_F(ZoneTest, testMassAdd10000) {
	massAdd(10000);
}

TEST_F(ZoneTest, testMassAdd100000) {
	massAdd(100000);
}

TEST_F(ZoneTest, testSingleAdd10) {
	singleAdd(10);
}

TEST_F(ZoneTest, testSingleAdd100) {
	singleAdd(100);
}

TEST_F(ZoneTest, testSingleAdd1000) {
	singleAdd(1000);
}

TEST_F(ZoneTest, testSingleAdd10000) {
	singleAdd(10000);
}

TEST_F(ZoneTest, testSingleAdd100000) {
	singleAdd(100000);
}
