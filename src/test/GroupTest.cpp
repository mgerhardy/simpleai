#include "GroupTest.h"

class GroupTest: public TestSuite {
};

TEST_F(GroupTest, testGroupAddRemove) {
	const ai::GroupId id = 1;
	ai::GroupMgr groupMgr;
	TestEntity entity1(1, ai::TreeNodePtr(), _pathfinder, groupMgr);
	ASSERT_TRUE(groupMgr.add(id, &entity1));
	ASSERT_FALSE(groupMgr.remove(0, &entity1));
	ASSERT_TRUE(groupMgr.remove(id, &entity1));
}

TEST_F(GroupTest, testGroupSize) {
	const ai::GroupId id = 1;
	ai::GroupMgr groupMgr;
	TestEntity entity1(1, ai::TreeNodePtr(), _pathfinder, groupMgr);
	ASSERT_TRUE(groupMgr.add(id, &entity1));
	TestEntity entity2(2, ai::TreeNodePtr(), _pathfinder, groupMgr);
	ASSERT_TRUE(groupMgr.add(id, &entity2));
	ASSERT_EQ(&entity1, *groupMgr.getGroupMembers(id).first);
	ASSERT_EQ(&entity2, *(++groupMgr.getGroupMembers(id).first));
}

TEST_F(GroupTest, testGroupAveragePosition) {
	const ai::GroupId id = 1;
	ai::GroupMgr groupMgr;
	TestEntity entity1(1, ai::TreeNodePtr(), _pathfinder, groupMgr);
	entity1.setPosition(ai::AIPosition(1.0f, 1.0f, 0.0f));
	ASSERT_TRUE(groupMgr.add(id, &entity1));
	TestEntity entity2(2, ai::TreeNodePtr(), _pathfinder, groupMgr);
	entity2.setPosition(ai::AIPosition(3.0f, 3.0f, 0.0f));
	ASSERT_TRUE(groupMgr.add(id, &entity2));
	ai::AIPosition avg = groupMgr.getPosition(id);
	ASSERT_EQ(ai::AIPosition(2.0f, 2.0f, 0.0f), avg);
}
