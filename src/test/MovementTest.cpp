#include "MovementTest.h"

class MovementTest: public TestSuite {
};

TEST_F(MovementTest, testFlee) {
	ai::movement::TargetFlee flee("0:0:0");
	TestEntity entity(1, ai::TreeNodePtr());

	// flee to the left
	entity.setPosition(ai::Vector3f(-1, 0, 0));
	const ai::MoveVector& mvLeft = flee.execute(entity, 100);
	ASSERT_EQ(ai::Vector3f(-100.0f, 0.0f, 0.0f), mvLeft.getVector());
	ASSERT_FLOAT_EQ(M_PI, mvLeft.getOrientation(1.0f));

	// flee to the right
	entity.setPosition(ai::Vector3f(1, 0, 0));
	const ai::MoveVector& mvRight = flee.execute(entity, 100);
	ASSERT_EQ(ai::Vector3f(100.0f, 0.0f, 0.0f), mvRight.getVector());
	ASSERT_FLOAT_EQ(0.0f, mvRight.getOrientation(1.0f));

	// flee into positive z
	entity.setPosition(ai::Vector3f(0, 0, 1));
	const ai::MoveVector& mvPosZ = flee.execute(entity, 100);
	ASSERT_EQ(ai::Vector3f(0.0f, 0.0f, 100.0f), mvPosZ.getVector());
	ASSERT_FLOAT_EQ(M_PI_2, mvPosZ.getOrientation(1.0f));

	// flee into negative z
	entity.setPosition(ai::Vector3f(0, 0, -1));
	const ai::MoveVector& mvNegZ = flee.execute(entity, 100);
	ASSERT_EQ(ai::Vector3f(0.0f, 0.0f, -100.0f), mvNegZ.getVector());
	ASSERT_FLOAT_EQ(M_PI_2 + M_PI, mvNegZ.getOrientation(1.0f));
}

TEST_F(MovementTest, testWanderWithoutOrientationChange) {
	ai::movement::Wander wander("0.0");
	TestEntity entity(1, ai::TreeNodePtr());

	// moving to the right
	entity.setOrientation(0.0f);
	const ai::MoveVector& mvRight = wander.execute(entity, 100);
	ASSERT_EQ(ai::Vector3f(100.0f, 0.0f, 0.0f), mvRight.getVector());
	ASSERT_EQ(0.0f, mvRight.getOrientation(1.0f));

	// moving to the left
	entity.setOrientation(M_PI);
	const ai::MoveVector& mvLeft = wander.execute(entity, 100);
	ASSERT_EQ(ai::Vector3f(-100.0f, 0.0f, 0.0f), mvLeft.getVector());
	ASSERT_EQ(0.0f, mvLeft.getOrientation(1.0f));

	// moving into positive z
	entity.setOrientation(M_PI_2);
	const ai::MoveVector& mvPosZ = wander.execute(entity, 100);
	ASSERT_EQ(ai::Vector3f(0.0f, 0.0f, 100.0f), mvPosZ.getVector());
	ASSERT_EQ(0.0f, mvPosZ.getOrientation(1.0f));

	// moving negative z
	entity.setOrientation(M_PI_2 + M_PI);
	const ai::MoveVector& mvNegZ = wander.execute(entity, 100);
	ASSERT_EQ(ai::Vector3f(0.0f, 0.0f, -100.0f), mvNegZ.getVector());
	ASSERT_EQ(0.0f, mvNegZ.getOrientation(1.0f));
}

TEST_F(MovementTest, testWeightedSteering) {
	ai::randomSeed(0);

	ai::Zone zone("movementTest");
	TestEntity entity(1, ai::TreeNodePtr());
	entity.setOrientation(0.0f);
	entity.setPosition(ai::Vector3f(0, 0, 0));
	zone.addAI(&entity.getAI());

	ai::SteeringPtr flee(new ai::movement::TargetFlee("1:0:0"));
	ai::SteeringPtr wander(new ai::movement::Wander("0"));

	ai::movement::WeightedSteerings s;
	s.push_back(ai::movement::WeightedData(flee, 0.8f));
	s.push_back(ai::movement::WeightedData(wander, 0.2f));

	ai::movement::WeightedSteering w(s);
	const ai::MoveVector& mv = w.execute(entity, 100.0f);
	ASSERT_EQ(M_PI, mv.getOrientation(1.0f));
	ASSERT_EQ(ai::Vector3f(-80.0f, 0.0f, 0.0f), mv.getVector());
}
