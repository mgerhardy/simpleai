#include "LUATreeLoaderTest.h"
#include <SimpleAI.h>
#include <tree/loaders/lua/LUATreeLoader.h>

namespace {
const char *TREE = "function init ()"
		"local example = AI.createTree(\"example\")"
		"local rootNodeExample1 = example:createRoot(\"PrioritySelector\", \"root1\")"
		"rootNodeExample1:addNode(\"Idle{3000}\", \"idle3000_1\"):setCondition(\"True\")"
		"local rootNodeExample2 = AI.createTree(\"example2\"):createRoot(\"PrioritySelector\", \"root2\")"
		"rootNodeExample2:addNode(\"Idle{3000}\", \"idle3000_2\"):setCondition(\"True\")"
		"rootNodeExample2:addNode(\"Steer{0.6,0.4}(GroupFlee{2},Wander{1})\", \"wander\")"
		"end";
}

class LUATreeLoaderTest: public TestSuite {
};

TEST_F(LUATreeLoaderTest, testLoad) {
	ai::AIRegistry registry;
	ai::LUATreeLoader loader(registry);
	ASSERT_TRUE(loader.init(TREE)) << loader.getError();
	{
		const ai::TreeNodePtr& tree = loader.load("example");
		ASSERT_NE(nullptr, tree.get()) << "Could not find the expected behaviour";
		ASSERT_EQ("root1", tree->getName()) << "unexpected root node name";
		const ai::TreeNodes& children = tree->getChildren();
		const int childrenAmount = children.size();
		ASSERT_EQ(1, childrenAmount) << "expected amount of children";
		ASSERT_EQ("idle3000_1", children[0]->getName()) << "unexpected child node name";
		ASSERT_EQ("True", children[0]->getCondition()->getName()) << "unexpected condition name";
	}
	{
		const ai::TreeNodePtr& tree = loader.load("example2");
		ASSERT_NE(nullptr, tree.get()) << "Could not find the expected behaviour";
		ASSERT_EQ("root2", tree->getName()) << "unexpected root node name";
		const ai::TreeNodes& children = tree->getChildren();
		const int childrenAmount = children.size();
		ASSERT_EQ(2, childrenAmount) << "expected amount of children";
		ASSERT_EQ("idle3000_2", children[0]->getName()) << "unexpected child node name";
		ASSERT_EQ("True", children[0]->getCondition()->getName()) << "unexpected condition name";
		ASSERT_EQ("wander", children[1]->getName()) << "unexpected child node name";
		ASSERT_EQ("True", children[0]->getCondition()->getName()) << "unexpected condition name";
	}
}
