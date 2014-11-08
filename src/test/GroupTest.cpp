#include "GroupTest.h"

class GroupMgrTest: public TestSuite {
protected:
	std::vector<SharedPtr<TestEntity> > _ais;
	ai::GroupMgr _groupMgr;
	const ai::GroupId _id = 1;
public:
	virtual void SetUp() override {
		TestSuite::SetUp();
		const int max = 10000;
		for (int i = 1; i <= max; ++i) {
			const ai::CharacterId id = i;
			TestEntity *e = new TestEntity(id, ai::TreeNodePtr(), _pathfinder, _groupManager);
			_ais.push_back(SharedPtr<TestEntity>(e));
			e->setPosition(ai::Vector3f(3.0f, 3.0f, 0.0f));
			_groupMgr.add(_id, e);
		}
	}
};

class GroupTest: public TestSuite {
public:
	void doMassTest(int max) {
		std::vector<SharedPtr<TestEntity> > ais;
		ai::GroupMgr mgr;
		for (int i = 1; i <= max; ++i) {
			const ai::CharacterId id = i;
			TestEntity *e = new TestEntity(id, ai::TreeNodePtr(), _pathfinder, _groupManager);
			ais.push_back(SharedPtr<TestEntity>(e));
			mgr.add(1, e);
		}
		mgr.update();
		const std::pair<ai::GroupMembersSetIter, ai::GroupMembersSetIter>& members = mgr.getGroupMembers(1);
		const int n = std::distance(members.first, members.second);
		ASSERT_EQ(max, n);

		ASSERT_EQ(max, mgr.getGroupSize(1));

		for (std::vector<SharedPtr<TestEntity> >::iterator i = ais.begin(); i != ais.end(); ++i) {
			mgr.remove(1, i->get());
		}
		mgr.update();
		const std::pair<ai::GroupMembersSetIter, ai::GroupMembersSetIter>& membersEmpty = mgr.getGroupMembers(1);
		const int nEmpty = std::distance(membersEmpty.first, membersEmpty.second);
		ASSERT_EQ(0, nEmpty);
	}
};

TEST_F(GroupTest, testGroupAddRemove) {
	const ai::GroupId id = 1;
	ai::GroupMgr groupMgr;
	TestEntity entity1(1, ai::TreeNodePtr(), _pathfinder, groupMgr);
	groupMgr.add(id, &entity1);
	groupMgr.update();
	ASSERT_TRUE(groupMgr.isInAnyGroup(entity1));
	groupMgr.remove(0, &entity1);
	groupMgr.update();
	ASSERT_TRUE(groupMgr.isInAnyGroup(entity1));
	groupMgr.remove(id, &entity1);
	groupMgr.update();
	ASSERT_FALSE(groupMgr.isInAnyGroup(entity1));
	groupMgr.remove(id, &entity1);
	groupMgr.update();
	ASSERT_FALSE(groupMgr.isInAnyGroup(entity1));
}

TEST_F(GroupTest, testGroupIsInAny) {
	const ai::GroupId id = 1;
	ai::GroupMgr groupMgr;
	TestEntity entity1(1, ai::TreeNodePtr(), _pathfinder, groupMgr);
	groupMgr.add(id, &entity1);
	groupMgr.update();
	ASSERT_TRUE(groupMgr.isInAnyGroup(entity1));
	groupMgr.remove(id, &entity1);
	groupMgr.update();
	ASSERT_FALSE(groupMgr.isInAnyGroup(entity1));
}

TEST_F(GroupTest, testGroupSize) {
	const ai::GroupId id = 1;
	ai::GroupMgr groupMgr;
	TestEntity entity1(1, ai::TreeNodePtr(), _pathfinder, groupMgr);
	groupMgr.add(id, &entity1);
	groupMgr.update();
	TestEntity entity2(2, ai::TreeNodePtr(), _pathfinder, groupMgr);
	groupMgr.add(id, &entity2);
	groupMgr.update();
	std::pair<ai::GroupMembersSetIter, ai::GroupMembersSetIter> members = groupMgr.getGroupMembers(id);
	ASSERT_EQ(2, std::distance(members.first, members.second));
	ASSERT_EQ(&entity1, *members.first);
	++members.first;
	ASSERT_NE(members.first, members.second);
	ASSERT_EQ(&entity2, *members.first);
}

TEST_F(GroupTest, testGroupAveragePosition) {
	const ai::GroupId id = 1;
	ai::Vector3f avg;
	ai::GroupMgr groupMgr;
	TestEntity entity1(1, ai::TreeNodePtr(), _pathfinder, groupMgr);
	entity1.setPosition(ai::Vector3f(1.0f, 1.0f, 0.0f));
	groupMgr.add(id, &entity1);
	groupMgr.update();
	avg = groupMgr.getPosition(id);
	ASSERT_EQ(ai::Vector3f(1.0f, 1.0f, 0.0f), avg);
	TestEntity entity2(2, ai::TreeNodePtr(), _pathfinder, groupMgr);
	entity2.setPosition(ai::Vector3f(3.0f, 3.0f, 0.0f));
	groupMgr.add(id, &entity2);
	groupMgr.update();
	avg = groupMgr.getPosition(id);
	ASSERT_EQ(ai::Vector3f(2.0f, 2.0f, 0.0f), avg);
}

TEST_F(GroupMgrTest, testMassGroupAveragePosition) {
	ai::Vector3f avg = _groupMgr.getPosition(_id);
	ASSERT_EQ(ai::Vector3f(3.0f, 3.0f, 0.0f), avg);
}

TEST_F(GroupTest, testGroupMass1000) {
	doMassTest(1000);
}

TEST_F(GroupTest, testGroupMass10000) {
	doMassTest(10000);
}
