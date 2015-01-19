#include "LUATreeLoaderTest.h"
#include <SimpleAI.h>

namespace {
const char *TREE = "function init ()"
		"local example = AI.createTree(\"example\")"
		"local rootNodeExample = example:createRoot(\"PrioritySelector\", \"root\")"
		"end";
}

class LUATreeLoaderTest: public TestSuite {
};

TEST_F(LUATreeLoaderTest, testLoad) {
	ai::AIRegistry registry;
	ai::LUATreeLoader loader(registry);
	ASSERT_TRUE(loader.init(TREE));
}
