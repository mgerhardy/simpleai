#include "MovementTest.h"

class MovementTest: public TestSuite {
};

TEST_F(MovementTest, testWander) {
	TestEntity entity(1, ai::TreeNodePtr(), _pathfinder, _groupManager);
	entity.setOrientation(1.0f);

	ai::movement::Wander w(entity, 10.0f, 8.0f * M_PI);
	ai::randomSeed(0);
	const ai::MoveVector& mv = w.execute();
	EXPECT_NEAR(11.2f, mv.getRotation(), 0.01f);
	EXPECT_NEAR(fmodf(11.2f, (float)ai::M_2PI), mv.getOrientation(1.0f), 0.01f);
	const ai::Vector3f& v = mv.getVector();
	const ai::Vector3f expected(8.41471f, 10.0f, 10.5403f);
	ASSERT_EQ(expected, v);
}
