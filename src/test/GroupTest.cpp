#include "GroupTest.h"

class GroupMgrTest: public TestSuite {
protected:
	std::vector<std::shared_ptr<TestEntity> > _ais;
	ai::GroupMgr _groupMgr;
	const ai::GroupId _id = 1;
public:
	virtual void SetUp() override {
		TestSuite::SetUp();
		const int max = 10000;
		_ais.reserve(max);
		for (int i = 1; i <= max; ++i) {
			const ai::CharacterId id = i;
			TestEntity *e = new TestEntity(id, ai::TreeNodePtr(), _groupManager);
			_ais.push_back(std::shared_ptr<TestEntity>(e));
			e->setPosition(ai::Vector3f(3.0f, 3.0f, 0.0f));
			_groupMgr.add(_id, e);
		}
	}
};

class GroupTest: public TestSuite {
public:
	void doMassTest(int max) {
		std::vector<std::shared_ptr<TestEntity> > ais(max);
		const ai::GroupId groupId = 1;
		ai::GroupMgr mgr;
		for (int i = 1; i <= max; ++i) {
			const ai::CharacterId id = i;
			TestEntity *e = new TestEntity(id, ai::TreeNodePtr(), _groupManager);
			ais.push_back(std::shared_ptr<TestEntity>(e));
			mgr.add(groupId, e);
		}
		ASSERT_EQ(max, mgr.getGroupSize(groupId));

		for (auto i = ais.begin(); i != ais.end(); ++i) {
			mgr.remove(1, i->get());
		}
		const int nEmpty = mgr.getGroupSize(groupId);
		ASSERT_EQ(0, nEmpty);
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
	std::vector<const ai::ICharacter*> entities;
	auto func = [&](const ai::ICharacter& chr) {
		entities.push_back(&chr);
		return true;
	};
	groupMgr.visit(id, func);
	ASSERT_EQ(&entity1, entities[0]);
	ASSERT_EQ(&entity2, entities[1]);
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
	ASSERT_TRUE(groupMgr.isGroupLeader(id, entity2));
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

TEST_F(GroupMgrTest, testMassGroupAveragePosition) {
	const ai::Vector3f& avg = _groupMgr.getPosition(_id);
	ASSERT_EQ(ai::Vector3f(3.0f, 3.0f, 0.0f), avg);
}

TEST_F(GroupTest, testGroupMass1000) {
	doMassTest(1000);
}

TEST_F(GroupTest, testGroupMass10000) {
	doMassTest(10000);
}
