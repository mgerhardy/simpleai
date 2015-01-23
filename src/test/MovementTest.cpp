#include "MovementTest.h"

class MovementTest: public TestSuite {
};

TEST_F(MovementTest, testWander) {
	ai::randomSeed(0);

	TestEntity entity(1, ai::TreeNodePtr());
	entity.setOrientation(1.0f);

	ai::movement::Wander w(entity, 10.0f, ai::toRadians(10.0f));
	const ai::MoveVector& mv = w.execute();
	EXPECT_NEAR(11.2f, mv.getRotation(), 0.01f);
	EXPECT_NEAR(fmodf(11.2f, (float)ai::M_2PI), mv.getOrientation(1.0f), 0.01f);
	const ai::Vector3f& v = mv.getVector();
	const ai::Vector3f expected(8.41471f, 0.0f, 5.40302f);
	ASSERT_EQ(expected, v);
}

TEST_F(MovementTest, testWeightedSteering) {
	ai::randomSeed(0);

	ai::Zone zone("movementTest");
	TestEntity entity(1, ai::TreeNodePtr());
	entity.setOrientation(1.0f);
	zone.addAI(&entity.getAI());

	ai::movement::GroupFlee flee(entity, 10.0f, 1);
	ai::movement::Wander wander(entity, 10.0f, 8.0f * M_PI);

	ai::movement::WeightedSteerings s;
	s.push_back(ai::movement::WeightedData(&flee, 0.8f));
	s.push_back(ai::movement::WeightedData(&wander, 0.2f));

	ai::movement::WeightedSteering w(s);
	const ai::MoveVector& mv = w.execute();
	EXPECT_NEAR(11.21f, mv.getRotation(), 0.01f);
	EXPECT_NEAR(fmodf(11.21f, (float)ai::M_2PI), mv.getOrientation(1.0f), 0.01f);
	const ai::Vector3f& v = mv.getVector();
	const ai::Vector3f expected(8.41471f, 0.0f, 5.40302f);
	ASSERT_EQ(expected, v);
}
