#include "LUATreeLoaderTest.h"
#include <SimpleAI.h>

class LUATreeLoaderTest: public TestSuite {
};

TEST_F(LUATreeLoaderTest, testLoad) {
	ai::AIRegistry registry;
	ai::LUATreeLoader loader(registry);
	ASSERT_TRUE(loader.init(""));
}
