#include "NodeTest.h"
#include <tree/Sequence.h>
#include <conditions/True.h>

class NodeTest: public TestSuite {
};

TEST_F(NodeTest, testSequence) {
	ai::Sequence::Factory f;
	ai::TreeNodeFactoryContext ctx("testsequence", "", ai::True::get());
	ai::TreeNodePtr seq = f.create(&ctx);
	//seq->addChild();
}
