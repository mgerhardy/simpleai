#include "XMLTreeLoaderTest.h"
#include <SimpleAI.h>

namespace {
const char *TREE = "<?xml version=\"1.0\" standalone=\"no\" ?>";
}

class XMLTreeLoaderTest: public TestSuite {
};

TEST_F(XMLTreeLoaderTest, testLoad) {
	ai::AIRegistry registry;
	ai::XMLTreeLoader loader(registry);
	ASSERT_TRUE(loader.init(TREE));
}
