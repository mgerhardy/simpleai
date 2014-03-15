#include "AggroTest.h"
#include <AI.h>

class AggroTest: public TestSuite {
public:
	void doMassTest(int max) {
		std::vector<SharedPtr<TestEntity> > ais;
		ai::AggroMgr mgr;
		for (int i = 1; i <= max; ++i) {
			const ai::CharacterId id = i;
			TestEntity *e = new TestEntity(id, ai::TreeNodePtr(), _pathfinder);
			ais.push_back(SharedPtr<TestEntity>(e));
			ai::Entry* entry = mgr.addAggro(*e, i);
			entry->setReduceByValue(i);
		}
		const ai::EntryPtr& entry = mgr.getHighestEntry();
		ASSERT_TRUE(entry)<< "Highest entry not set but aggro was added";
		ASSERT_EQ(max, entry->getCharacterId())<< "Highest entry not what it should be. " << printAggroList(mgr);
		mgr.update(1000);
		ASSERT_EQ(0u, mgr.getEntries().size());
		ASSERT_TRUE(mgr.getEntries().empty());
	}
};

TEST_F(AggroTest, testAggroMgr) {
	ai::AggroMgr mgr;
	const ai::CharacterId id = 1;
	TestEntity entity(id, ai::TreeNodePtr(), _pathfinder);
	mgr.addAggro(entity, 1.0f);
	const ai::EntryPtr& entry = mgr.getHighestEntry();
	ASSERT_TRUE(entry)<< "Highest entry not set but aggro was added";
	ASSERT_EQ(id, entry->getCharacterId())<< "Highest entry not what it should be";
	mgr.addAggro(entity, 1.0f);
	ASSERT_EQ(1u, mgr.getEntries().size())<< "Aggrolist contains more entries than expected";
	ASSERT_FLOAT_EQ(2.0f, entry->getAggro())<< "Aggro value not what it should be";
}

TEST_F(AggroTest, testAggroMgr50) {
	doMassTest(50);
}

TEST_F(AggroTest, testAggroMgr5000) {
	doMassTest(5000);
}

TEST_F(AggroTest, testAggroMgr50000) {
	doMassTest(50000);
}

TEST_F(AggroTest, testAggroMgr500000) {
	doMassTest(500000);
}

TEST_F(AggroTest, testAggroMgrDegradeValue) {
	const float expectedAggro = 1.0f;
	const int seconds = 2;
	const float reduceBySecond = 0.1f;
	ai::AggroMgr mgr;
	const ai::CharacterId id = 1;
	TestEntity entity(id, ai::TreeNodePtr(), _pathfinder);
	mgr.addAggro(entity, expectedAggro);
	const ai::EntryPtr& entry = mgr.getHighestEntry();
	entry->setReduceByValue(reduceBySecond);
	ASSERT_TRUE(entry)<< "Highest entry not set but aggro was added";
	ASSERT_EQ(id, entry->getCharacterId())<< "Highest entry not what it should be";
	const float aggro = entry->getAggro();
	ASSERT_FLOAT_EQ(expectedAggro, aggro);
	mgr.update(seconds * 1000);
	const float expected = expectedAggro - seconds * reduceBySecond;
	const float newAggro = entry->getAggro();
	ASSERT_FLOAT_EQ(expected, newAggro);
}
