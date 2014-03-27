#include "GeneralTest.h"

class GeneralTest: public TestSuite {
};

TEST_F(GeneralTest, testAIPosition) {
	const ai::AIPosition posNe1(1.0f, 1.0f, 1.0f);
	const ai::AIPosition posNe2(2.0f, 1.0f, 1.0f);
	ASSERT_NE(posNe1, posNe2);

	const ai::AIPosition pos1(1.0f, 1.0f, 1.0f);
	const ai::AIPosition pos2(1.0f, 1.0f, 1.0f);
	ASSERT_EQ(pos1, pos2);
}
