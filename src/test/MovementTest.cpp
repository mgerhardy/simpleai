#include "MovementTest.h"

class MovementTest: public TestSuite {
};

TEST_F(MovementTest, testWander) {
	ai::randomSeed(0);

	TestEntity entity(1, ai::TreeNodePtr());
	entity.setOrientation(1.0f);

	const float degrees = 10.0f;
	const float radians = ai::toRadians(degrees);
	ai::movement::Wander w(std::to_string(radians));
	const ai::MoveVector& mv = w.execute(entity, 10.0f);
	EXPECT_NEAR(0.0f, mv.getRotation(), radians);
	EXPECT_NEAR(0.0f, mv.getOrientation(1.0f), radians);
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

	const float degrees = 10.0f;
	const float radians = ai::toRadians(degrees);

	ai::SteeringPtr flee(new ai::movement::GroupFlee("1"));
	ai::SteeringPtr wander(new ai::movement::Wander(std::to_string(radians)));

	ai::movement::WeightedSteerings s;
	s.push_back(ai::movement::WeightedData(flee, 0.8f));
	s.push_back(ai::movement::WeightedData(wander, 0.2f));

	ai::movement::WeightedSteering w(s);
	const ai::MoveVector& mv = w.execute(entity, 10.0f);
	EXPECT_NEAR(0.0f, mv.getRotation(), radians);
	EXPECT_NEAR(0.0f, mv.getOrientation(1.0f), radians);
	const ai::Vector3f& v = mv.getVector();
	const ai::Vector3f expected(8.41471f, 0.0f, 5.40302f);
	ASSERT_EQ(expected, v);
}
