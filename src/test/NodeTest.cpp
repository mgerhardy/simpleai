#include "NodeTest.h"

class NodeTest: public TestSuite {
};

TEST_F(NodeTest, testSequence) {
	ai::Sequence::Factory f;
	ai::TreeNodeFactoryContext ctx("testsequence", "", ai::True::get());
	ai::TreeNodePtr node = f.create(&ctx);

	ai::Idle::Factory idleFac;
	ai::TreeNodeFactoryContext idleCtx1("testidle", "2", ai::True::get());
	ai::TreeNodePtr idle1 = idleFac.create(&idleCtx1);
	ai::TreeNodeFactoryContext idleCtx2("testidle2", "2", ai::True::get());
	ai::TreeNodePtr idle2 = idleFac.create(&idleCtx2);

	node->addChild(idle1);
	node->addChild(idle2);

	TestEntity e(1, node, _pathfinder);
	e.update(1);
	ASSERT_EQ(ai::RUNNING, idle1->getLastStatus());
	ASSERT_EQ(ai::UNKNOWN, idle2->getLastStatus());
	e.update(1);
	ASSERT_EQ(ai::RUNNING, idle1->getLastStatus());
	ASSERT_EQ(ai::UNKNOWN, idle2->getLastStatus());
	e.update(1);
	ASSERT_EQ(ai::FINISHED, idle1->getLastStatus());
	ASSERT_EQ(ai::RUNNING, idle2->getLastStatus());
	e.update(1);
	ASSERT_EQ(ai::FINISHED, idle1->getLastStatus());
	ASSERT_EQ(ai::RUNNING, idle2->getLastStatus());
	e.update(1);
	ASSERT_EQ(ai::FINISHED, idle1->getLastStatus());
	ASSERT_EQ(ai::FINISHED, idle2->getLastStatus());
	e.update(1);
	ASSERT_EQ(ai::RUNNING, idle1->getLastStatus());
	ASSERT_EQ(ai::FINISHED, idle2->getLastStatus());
}

TEST_F(NodeTest, testIdle) {
	ai::Idle::Factory f;
	ai::TreeNodeFactoryContext ctx("testidle", "1000", ai::True::get());
	ai::TreeNodePtr node = f.create(&ctx);
	TestEntity entity(1, ai::TreeNodePtr(), _pathfinder);
	ASSERT_EQ(ai::RUNNING, node->execute(entity, 1));
	ASSERT_EQ(ai::FINISHED, node->execute(entity, 1000));
}
