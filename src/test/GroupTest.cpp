#include "GroupTest.h"

class GroupMgrTest: public TestSuite {
};

TEST_F(GroupMgrTest, testMassGroupAveragePosition) {
	std::vector<std::shared_ptr<TestEntity> > ais;
	const ai::GroupId groupId = 1;
	const ai::Vector3f pos1(3.0f, 3.0f, 0.0f);
	const ai::Vector3f pos2(300.0f, 300.0f, 0.0f);
	for (int i = 1; i <= 2; ++i) {
		const ai::CharacterId id = i;
		TestEntity *e = new TestEntity(id, ai::TreeNodePtr(), _groupManager);
		ais.push_back(std::shared_ptr<TestEntity>(e));
		e->setPosition(pos1);
		_groupManager.add(groupId, e);
	}
	for (int i = 3; i <= 4; ++i) {
		const ai::CharacterId id = i;
		TestEntity *e = new TestEntity(id, ai::TreeNodePtr(), _groupManager);
		ais.push_back(std::shared_ptr<TestEntity>(e));
		e->setPosition(pos2);
		_groupManager.add(groupId, e);
	}
	const ai::Vector3f& avg = _groupManager.getPosition(groupId);
	const ai::Vector3f pos = pos1 + pos2;

	ASSERT_EQ(0.5f * pos, avg);
}

class GroupTest: public TestSuite {
private:
public:
	typedef std::vector<std::shared_ptr<TestEntity> > TestEntities;
	inline void addMass(int max, ai::GroupId groupId, TestEntities& ais, ai::GroupMgr& mgr) const {
		for (int i = 1; i <= max; ++i) {
			const ai::CharacterId id = i;
			TestEntity *e = new TestEntity(id, ai::TreeNodePtr(), mgr);
			ais.push_back(std::shared_ptr<TestEntity>(e));
			mgr.add(groupId, e);
		}
		ASSERT_EQ(max, mgr.getGroupSize(groupId));
	}

	inline void remove(ai::GroupId groupId, TestEntities& ais, ai::GroupMgr& mgr) const {
		for (auto i = ais.begin(); i != ais.end(); ++i) {
			mgr.remove(groupId, i->get());
		}
		const int nEmpty = mgr.getGroupSize(groupId);
		ASSERT_EQ(0, nEmpty) << "Unexpected group size for " << groupId;
	}

	void doMassTest(int max) {
		TestEntities ais(max);
		const ai::GroupId groupId = 1;
		ai::GroupMgr mgr;

		addMass(max, groupId, ais, mgr);
		remove(groupId, ais, mgr);
	}
};

TEST_F(GroupTest, testGroupAddRemove) {
	const ai::GroupId id = 1;
	ai::GroupMgr groupMgr;
	TestEntity entity1(1, ai::TreeNodePtr(), groupMgr);
	ASSERT_TRUE(groupMgr.add(id, &entity1));
	ASSERT_FALSE(groupMgr.remove(0, &entity1));
	ASSERT_TRUE(groupMgr.remove(id, &entity1));
	ASSERT_FALSE(groupMgr.remove(id, &entity1));
}

TEST_F(GroupTest, testGroupIsInAny) {
	const ai::GroupId id = 1;
	ai::GroupMgr groupMgr;
	TestEntity entity1(1, ai::TreeNodePtr(), groupMgr);
	ASSERT_TRUE(groupMgr.add(id, &entity1));
	ASSERT_TRUE(groupMgr.isInAnyGroup(entity1));
	ASSERT_TRUE(groupMgr.remove(id, &entity1));
	ASSERT_FALSE(groupMgr.isInAnyGroup(entity1));
}

TEST_F(GroupTest, testGroupSize) {
	const ai::GroupId id = 1;
	ai::GroupMgr groupMgr;
	TestEntity entity1(1, ai::TreeNodePtr(), groupMgr);
	ASSERT_TRUE(groupMgr.add(id, &entity1));
	TestEntity entity2(2, ai::TreeNodePtr(), groupMgr);
	ASSERT_TRUE(groupMgr.add(id, &entity2));
	ASSERT_EQ(2, groupMgr.getGroupSize(id));
}

TEST_F(GroupTest, testGroupLeader) {
	const ai::GroupId id = 1;
	ai::GroupMgr groupMgr;
	TestEntity entity1(1, ai::TreeNodePtr(), groupMgr);
	ASSERT_TRUE(groupMgr.add(id, &entity1));
	TestEntity entity2(2, ai::TreeNodePtr(), groupMgr);
	ASSERT_TRUE(groupMgr.add(id, &entity2));
	TestEntity entity3(3, ai::TreeNodePtr(), groupMgr);
	ASSERT_TRUE(groupMgr.add(id, &entity3));
	ASSERT_EQ(3, groupMgr.getGroupSize(id));
	ASSERT_TRUE(groupMgr.isGroupLeader(id, entity1));
	ASSERT_FALSE(groupMgr.isGroupLeader(id, entity2));
	ASSERT_FALSE(groupMgr.isGroupLeader(id, entity3));
	ASSERT_TRUE(groupMgr.remove(id, &entity1));
	ASSERT_FALSE(groupMgr.isInGroup(id, entity1));
}

TEST_F(GroupTest, testGroupAveragePosition) {
	const ai::GroupId id = 1;
	ai::Vector3f avg;
	ai::GroupMgr groupMgr;
	TestEntity entity1(1, ai::TreeNodePtr(), groupMgr);
	entity1.setPosition(ai::Vector3f(1.0f, 1.0f, 0.0f));
	ASSERT_TRUE(groupMgr.add(id, &entity1));
	avg = groupMgr.getPosition(id);
	ASSERT_EQ(ai::Vector3f(1.0f, 1.0f, 0.0f), avg);
	TestEntity entity2(2, ai::TreeNodePtr(), groupMgr);
	entity2.setPosition(ai::Vector3f(3.0f, 3.0f, 0.0f));
	ASSERT_TRUE(groupMgr.add(id, &entity2));
	avg = groupMgr.getPosition(id);
	ASSERT_EQ(ai::Vector3f(2.0f, 2.0f, 0.0f), avg);
}

TEST_F(GroupTest, testGroupMass1000) {
	doMassTest(1000);
}

TEST_F(GroupTest, testGroupMass10000) {
	doMassTest(10000);
}

class GroupMassTest: public GroupTest {
protected:
	GroupTest::TestEntities _ais;
	const int _maxUsers = 100;
	const int _maxGroups = 100;
public:
	virtual void SetUp() override {
		GroupTest::SetUp();
		_ais.clear();
		_ais.reserve(_maxUsers * _maxGroups);
		for (int i = 0; i < _maxGroups;++i) {
			const ai::GroupId groupId = i;
			addMass(_maxUsers, groupId, _ais, _groupManager);
		}
	}

	virtual void TearDown() override {
		GroupTest::TearDown();
		for (int i = 0; i < _maxGroups;++i) {
			const ai::GroupId groupId = i;
			remove(groupId, _ais, _groupManager);
		}
	}
};

TEST_F(GroupMassTest, testIsInAnyGroupMass100x100) {
	std::shared_ptr<TestEntity> e = _ais.back();
	ASSERT_TRUE(_groupManager.isInAnyGroup(*e));
}
