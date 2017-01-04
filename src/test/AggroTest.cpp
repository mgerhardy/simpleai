#include "AggroTest.h"

class AggroTest: public TestSuite {
protected:
	static std::vector<ai::AIPtr> _v;

	static void SetUpTestCase() {
		ai::TreeNodePtr root = std::make_shared<ai::PrioritySelector>("test", "", ai::True::get());
		const int n = 10000;
		_v.reserve(n);
		for (int i = 0; i < n; ++i) {
			ai::ICharacterPtr character = std::make_shared<TestEntity>(i + 1);
			ai::AIPtr ai = std::make_shared<ai::AI>(root);
			ai->setPause(true);
			ai->setCharacter(character);
			_v.push_back(ai);
		}
	}

	static void TearDownTestCase() {
		_v.clear();
	}

	void doMassTest(int max) {
		ai::AggroMgr mgr(max);
		ASSERT_TRUE(max <= _v.size());
		for (int i = 1; i <= max; ++i) {
			ai::Entry* entry = mgr.addAggro(_v[i - 1]->getId(), (float)i);
			entry->setReduceByValue((float)i);
		}
		const ai::EntryPtr& entry = mgr.getHighestEntry();
		ASSERT_TRUE((bool)entry) << "Highest entry not set but aggro was added";
		ASSERT_EQ(max, entry->getCharacterId())<< "Highest entry not what it should be. " << printAggroList(mgr);
		mgr.update(1000);
		ASSERT_EQ(0u, mgr.getEntries().size());
		ASSERT_TRUE(mgr.getEntries().empty());
	}
};

std::vector<ai::AIPtr> AggroTest::_v;

TEST_F(AggroTest, testAggroMgr) {
	ai::AggroMgr mgr;
	ai::AIPtr ai = _v[0];
	ai::CharacterId id = ai->getId();
	mgr.addAggro(id, 1.0f);
	const ai::EntryPtr& entry = mgr.getHighestEntry();
	ASSERT_TRUE((bool)entry) << "Highest entry not set but aggro was added";
	ASSERT_EQ(id, entry->getCharacterId())<< "Highest entry not what it should be";
	mgr.addAggro(id, 1.0f);
	ASSERT_EQ(1u, mgr.getEntries().size())<< "Aggrolist contains more entries than expected";
	ASSERT_FLOAT_EQ(2.0f, entry->getAggro())<< "Aggro value not what it should be";
}

TEST_F(AggroTest, testAggroMgr50) {
	doMassTest(50);
}

TEST_F(AggroTest, testAggroMgr500) {
	doMassTest(500);
}

TEST_F(AggroTest, testAggroMgr5000) {
	doMassTest(5000);
}

TEST_F(AggroTest, testAggroMgr10000) {
	doMassTest(10000);
}

TEST_F(AggroTest, testAggroMgrDegradeValue) {
	const float expectedAggro = 1.0f;
	const int seconds = 2;
	const float reduceBySecond = 0.1f;
	ai::AggroMgr mgr;
	ai::CharacterId id = _v[0]->getId();
	mgr.addAggro(id, expectedAggro);
	const ai::EntryPtr& entry = mgr.getHighestEntry();
	entry->setReduceByValue(reduceBySecond);
	ASSERT_TRUE(entry) << "Highest entry not set but aggro was added";
	ASSERT_EQ(id, entry->getCharacterId())<< "Highest entry not what it should be";
	const float aggro = entry->getAggro();
	ASSERT_FLOAT_EQ(expectedAggro, aggro);
	mgr.update(seconds * 1000);
	const float expected = expectedAggro - seconds * reduceBySecond;
	const float newAggro = entry->getAggro();
	ASSERT_FLOAT_EQ(expected, newAggro);
}
