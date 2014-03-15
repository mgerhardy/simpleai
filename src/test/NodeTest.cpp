#include "NodeTest.h"
#include <tree/Sequence.h>

class NodeTest: public TestSuite {
};

TEST_F(NodeTest, testSequence) {
	ai::Sequence::Factory f;
	ai::TreeNodeFactoryContext ctx("testsequence");
	ai::TreeNodePtr seq = f.create(&ctx);
	//seq->addChild();
}
