#include "LUATreeLoaderTest.h"
#include <SimpleAI.h>
#include <tree/loaders/lua/LUATreeLoader.h>

namespace {
const char *TREE = "function init ()\n"
		"local example = AI.createTree(\"example\")\n"
		"local rootNodeExample = example:createRoot(\"PrioritySelector\", \"root\")\n"
		"rootNodeExample:addNode(\"Idle{3000}\", \"idle3000\"):setCondition(\"True\")\n"
		"AI.createTree(\"wander\"):createRoot(\"Steer(Wander)\", \"wander\")\n"
		"AI.createSubTree(\"subexample\"):createRoot(\"Idle{$1}\", \"wander\")\n"
		"AI.createTree(\"usesubexample\"):createRoot(\"Slot{3000}(subexample)\", \"slotsubexample1\")\n"
		"end\n";
}

class LUATreeLoaderTest: public TestSuite {
};

TEST_F(LUATreeLoaderTest, testLoad) {
	ai::AIRegistry registry;
	ai::LUATreeLoader loader(registry);
	ASSERT_TRUE(loader.init(TREE)) << loader.getError() << " - Could not load: " << TREE;
	const ai::TreeNodePtr& tree = loader.load("example");
	ASSERT_NE(nullptr, tree.get()) << "Could not find the expected behaviour";
	ASSERT_EQ("root", tree->getName()) << "unexpected root node name";
	const ai::TreeNodes& children = tree->getChildren();
	const int childrenAmount = children.size();
	ASSERT_EQ(1, childrenAmount) << "expected amount of children";
	ASSERT_EQ("idle3000", children[0]->getName()) << "unexpected child node name";
	ASSERT_EQ("True", children[0]->getCondition()->getName()) << "unexpected condition name";
}
