#include "XMLTreeLoaderTest.h"
#include <SimpleAI.h>

class XMLTreeLoaderTest: public TestSuite {
};

TEST_F(XMLTreeLoaderTest, testLoad) {
	ai::AIRegistry registry;
	ai::XMLTreeLoader loader(registry);
	ASSERT_TRUE(loader.init(""));
}
