#include "NodeTest.h"

class NodeTest: public TestSuite {
};

TEST_F(NodeTest, testSequence) {
	ai::Sequence::Factory f;
	ai::TreeNodeFactoryContext ctx("testsequence", "", ai::True::get());
	ai::TreeNodePtr node = f.create(&ctx);
	//node->addChild();
}

TEST_F(NodeTest, testIdle) {
	ai::Idle::Factory f;
	ai::TreeNodeFactoryContext ctx("testidle", "1000", ai::True::get());
	ai::TreeNodePtr node = f.create(&ctx);
	TestEntity entity(1, ai::TreeNodePtr(), _pathfinder);
	ASSERT_EQ(ai::RUNNING, node->execute(entity, 1));
	ASSERT_EQ(ai::FINISHED, node->execute(entity, 1000));
}
