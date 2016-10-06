#include "XMLTreeLoaderTest.h"

namespace {
const char *TREE = "<?xml version=\"1.0\" standalone=\"no\" ?>"
	"<trees>"
		"<tree name=\"example1\">"
			"<node type=\"PrioritySelector\" name=\"root1\">"
				"<node type=\"Idle{3000}\" name=\"idle1\" condition=\"HasEnemies{3}\" />"
			"</node>"
		"</tree>"
		"<tree name=\"example2\">"
			"<node type=\"PrioritySelector\" name=\"root2\">"
				"<node type=\"Idle{3000}\" name=\"idle2\" />"
				"<node type=\"Steer{0.6,0.4}(GroupFlee{2},Wander{1})\" name=\"wander\" />"
			"</node>"
		"</tree>"
	"</trees>";
}

class XMLTreeLoaderTest: public TestSuite {
protected:
	ai::AIRegistry _registry;
	ai::XMLTreeLoader _loader;

public:
	XMLTreeLoaderTest() :
			_loader(_registry) {
	}

	void SetUp() override {
		TestSuite::SetUp();
		ASSERT_TRUE(_loader.init(TREE)) << _loader.getError();
	}

	void TearDown() override {
		TestSuite::TearDown();
		_loader.shutdown();
	}
};

TEST_F(XMLTreeLoaderTest, testLoadExample1) {
	const ai::TreeNodePtr& node = _loader.load("example1");
	ASSERT_NE(nullptr, node.get()) << "Could not find the espected behaviour";
	ASSERT_EQ("root1", node->getName()) << "unexpected root node name";
	const ai::TreeNodes& children = node->getChildren();
	const int childrenAmount = children.size();
	ASSERT_EQ(1, childrenAmount) << "expected amount of children";
	ASSERT_EQ("idle1", children[0]->getName()) << "unexpected child node name";
	ASSERT_NE(nullptr, children[0]->getCondition()) << "condition not parsed";
	ASSERT_EQ("HasEnemies", children[0]->getCondition()->getName()) << "unexpected condition name";
}

TEST_F(XMLTreeLoaderTest, testLoadExample2) {
	const ai::TreeNodePtr& node = _loader.load("example2");
	ASSERT_NE(nullptr, node.get()) << "Could not find the espected behaviour";
	ASSERT_EQ("root2", node->getName()) << "unexpected root node name";
	const ai::TreeNodes& children = node->getChildren();
	const int childrenAmount = children.size();
	ASSERT_EQ(2, childrenAmount) << "expected amount of children";
}
