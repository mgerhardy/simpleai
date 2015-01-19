#include "XMLTreeLoaderTest.h"
#include <SimpleAI.h>

namespace {
const char *TREE = "<?xml version=\"1.0\" standalone=\"no\" ?>"
	"<behaviours>"
		"<behaviour name=\"example1\">"
			"<node type=\"PrioritySelector\" name=\"root\">"
				"<node type=\"Idle\" param=\"3000\" name=\"idle\" condition=\"True\" conditionparam=\"\" />"
			"</node>"
		"</behaviour>"
		"<behaviour name=\"example2\">"
			"<node type=\"PrioritySelector\" name=\"root\">"
				"<node type=\"Idle\" name=\"idle\" />"
			"</node>"
		"</behaviour>"
	"</behaviours>";
}

class XMLTreeLoaderTest: public TestSuite {
};

TEST_F(XMLTreeLoaderTest, testLoad) {
	ai::AIRegistry registry;
	ai::XMLTreeLoader loader(registry);
	ASSERT_TRUE(loader.init(TREE)) << loader.getError();
	const ai::TreeNodePtr& tree = loader.load("example1");
	ASSERT_NE(nullptr, tree.get()) << "Could not find the espected behaviour";
	ASSERT_EQ("root", tree->getName()) << "unexpected root node name";
	const ai::TreeNodes& children = tree->getChildren();
	const int childrenAmount = children.size();
	ASSERT_EQ(1, childrenAmount) << "expected amount of children";
	ASSERT_EQ("idle", children[0]->getName()) << "unexpected child node name";
	ASSERT_NE(nullptr, children[0]->getCondition()) << "condition not parsed";
	ASSERT_EQ("True", children[0]->getCondition()->getName()) << "unexpected condition name";
	ASSERT_NE(nullptr, loader.load("example2")) << "Could not find the espected behaviour";
}
