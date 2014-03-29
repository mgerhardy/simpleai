#include "GeneralTest.h"

class GeneralTest: public TestSuite {
};

TEST_F(GeneralTest, testVector3f) {
	const ai::Vector3f posNe1(1.0f, 1.0f, 1.0f);
	const ai::Vector3f posNe2(2.0f, 1.0f, 1.0f);
	ASSERT_NE(posNe1, posNe2);

	const ai::Vector3f pos1(1.0f, 1.0f, 1.0f);
	const ai::Vector3f pos2(1.0f, 1.0f, 1.0f);
	ASSERT_EQ(pos1, pos2);
}
