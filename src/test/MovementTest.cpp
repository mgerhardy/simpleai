#include "MovementTest.h"

class MovementTest: public TestSuite {
};

TEST_F(MovementTest, testFlee) {
	ai::movement::TargetFlee flee("0:0:100");
	TestEntity entity(1, ai::TreeNodePtr());

	// flee to the left
	entity.setPosition(ai::Vector3f(0, 0, 0));
	const ai::MoveVector& mvLeft = flee.execute(entity, 100);
	ASSERT_EQ(ai::Vector3f(0.0f, 0.0f, -100.0f), mvLeft.getVector());
	ASSERT_FLOAT_EQ(M_PI, mvLeft.getOrientation(1.0f));

	// flee to the right
	entity.setPosition(ai::Vector3f(0, 0, 200));
	const ai::MoveVector& mvRight = flee.execute(entity, 100);
	ASSERT_EQ(ai::Vector3f(0.0f, 0.0f, 100.0f), mvRight.getVector());
	ASSERT_FLOAT_EQ(0.0f, mvRight.getOrientation(1.0f));
}

TEST_F(MovementTest, testWanderWithoutOrientationChange) {
	ai::movement::Wander wander("0.0");
	TestEntity entity(1, ai::TreeNodePtr());
	// moving to the right
	entity.setOrientation(0.0f);
	const ai::MoveVector& mvRight = wander.execute(entity, 100);
	ASSERT_EQ(ai::Vector3f(0.0f, 0.0f, 100.0f), mvRight.getVector());
	ASSERT_EQ(0.0f, mvRight.getOrientation(1.0f));

	// moving to the left
	entity.setOrientation(M_PI);
	const ai::MoveVector& mvLeft = wander.execute(entity, 100);
	ASSERT_EQ(ai::Vector3f(0.0f, 0.0f, -100.0f), mvLeft.getVector());
	ASSERT_EQ(0.0f, mvLeft.getOrientation(1.0f));

	// moving upwards
	entity.setOrientation(M_PI_2);
	const ai::MoveVector& mvUp = wander.execute(entity, 100);
	ASSERT_EQ(ai::Vector3f(100.0f, 0.0f, 0.0f), mvUp.getVector());
	ASSERT_EQ(0.0f, mvUp.getOrientation(1.0f));

	// moving downwards
	entity.setOrientation(M_PI_2 + M_PI);
	const ai::MoveVector& mvDown = wander.execute(entity, 100);
	ASSERT_EQ(ai::Vector3f(-100.0f, 0.0f, 0.0f), mvDown.getVector());
	ASSERT_EQ(0.0f, mvDown.getOrientation(1.0f));
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
