#include "XMLTreeLoaderTest.h"
#include <SimpleAI.h>

namespace {
const char *TREE = "<?xml version=\"1.0\" standalone=\"no\" ?>"
		"<behaviours>"
		"<node type=\"PrioritySelector\" name=\"example1\">"
		"<node type=\"Idle\" name=\"example1-idle\" />"
		"</node>"
		"<node type=\"PrioritySelector\" name=\"example2\">"
		"<node type=\"Idle\" name=\"example2-idle\" />" // TODO: condition
		"</node>"
		"</behaviours>";
}

class XMLTreeLoaderTest: public TestSuite {
};

TEST_F(XMLTreeLoaderTest, testLoad) {
	ai::AIRegistry registry;
	ai::XMLTreeLoader loader(registry);
	ASSERT_TRUE(loader.init(TREE)) << loader.getError();
}
