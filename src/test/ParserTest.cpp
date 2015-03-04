#include "ParserTest.h"

class ParserTest: public TestSuite {
};

class TestLoader : public ai::ITreeLoader {
public:
	TestLoader(ai::AIRegistry& registry) :
		ai::ITreeLoader(registry) {
	}
};

TEST_F(ParserTest, testParseConditionSimple) {
	ai::ConditionParser parser(_registry, "HasEnemies");
	const ai::ConditionPtr& c = parser.getCondition();
	ASSERT_NE(nullptr, c.get()) << parser.getError();
}

TEST_F(ParserTest, testParseConditionNot) {
	ai::ConditionParser parser(_registry, "Not(HasEnemies)");
	const ai::ConditionPtr& c = parser.getCondition();
	ASSERT_NE(nullptr, c.get()) << parser.getError();
}

TEST_F(ParserTest, testParseConditionAndNot) {
	ai::ConditionParser parser(_registry, "And(Not(HasEnemies),True)");
	const ai::ConditionPtr& c = parser.getCondition();
	ASSERT_NE(nullptr, c.get()) << parser.getError();
}

TEST_F(ParserTest, testParseConditionAndNotInnerParam) {
	ai::ConditionParser parser(_registry, "And(Not(HasEnemies{3}),True)");
	const ai::ConditionPtr& c = parser.getCondition();
	ASSERT_NE(nullptr, c.get()) << parser.getError();
}

TEST_F(ParserTest, testParseConditionAndNotInnerOuterParam) {
	ai::ConditionParser parser(_registry, "And(Not{3}(HasEnemies{3}),True)");
	const ai::ConditionPtr& c = parser.getCondition();
	ASSERT_NE(nullptr, c.get()) << parser.getError();
}

TEST_F(ParserTest, testParseConditionAndWithNot) {
	ai::ConditionParser parser(_registry, "And(Not(HasEnemies),Not(HasEnemies))");
	const ai::ConditionPtr& c = parser.getCondition();
	ASSERT_NE(nullptr, c.get()) << parser.getError();
}

TEST_F(ParserTest, testParseConditionParmEverywhere) {
	ai::ConditionParser parser(_registry, "And{1}(Not{3}(HasEnemies{3}),True{1})");
	const ai::ConditionPtr& c = parser.getCondition();
	ASSERT_NE(nullptr, c.get()) << parser.getError();
}

TEST_F(ParserTest, testParseFail) {
	ai::ConditionParser parser(_registry, "And(Not(HasEnemies{3},True)");
	const ai::ConditionPtr& c = parser.getCondition();
	ASSERT_FALSE(c) << parser.getError();
}

TEST_F(ParserTest, testParseConditionNodeMultipleParamsAsChild) {
	ai::ConditionParser parser(_registry, "Not(IsCloseToGroup{1,10})");
	const ai::ConditionPtr& c = parser.getCondition();
	ASSERT_NE(nullptr, c.get()) << parser.getError();
}

TEST_F(ParserTest, testParseTreeNode) {
	TestLoader loader(_registry);
	ai::TreeNodeParser parser(loader, "Invert{1}");
	const ai::TreeNodePtr& c = parser.getTreeNode();
	ASSERT_NE(nullptr, c.get()) << parser.getError();
}

TEST_F(ParserTest, testParseTreeNodeMultipleParams) {
	TestLoader loader(_registry);
	ai::TreeNodeParser parser(loader, "Invert{1,1000}");
	const ai::TreeNodePtr& c = parser.getTreeNode();
	ASSERT_NE(nullptr, c.get()) << parser.getError();
}

TEST_F(ParserTest, testParseIdleNode) {
	TestLoader loader(_registry);
	ai::TreeNodeParser parser(loader, "Idle{1000}");
	const ai::TreeNodePtr& c = parser.getTreeNode();
	ASSERT_NE(nullptr, c.get()) << parser.getError();
}

TEST_F(ParserTest, testParseIdleNodeNoParam) {
	TestLoader loader(_registry);
	ai::TreeNodeParser parser(loader, "Idle");
	const ai::TreeNodePtr& c = parser.getTreeNode();
	ASSERT_NE(nullptr, c.get()) << parser.getError();
}

TEST_F(ParserTest, testParseUnknown) {
	TestLoader loader(_registry);
	ai::TreeNodeParser parser(loader, "Unknown");
	const ai::TreeNodePtr& c = parser.getTreeNode();
	ASSERT_EQ(nullptr, c.get()) << parser.getError();
}

TEST_F(ParserTest, testFilterMissingFilterType) {
	ai::ConditionParser parser(_registry, "Filter");
	const ai::ConditionPtr& c = parser.getCondition();
	ASSERT_EQ("missing details for Filter condition", parser.getError());
	ASSERT_EQ(nullptr, c.get()) << parser.getError();
}

TEST_F(ParserTest, testFilter) {
	ai::ConditionParser parser(_registry, "Filter(SelectEmpty)");
	const ai::ConditionPtr& c = parser.getCondition();
	ASSERT_NE(nullptr, c.get()) << parser.getError();
}

TEST_F(ParserTest, testMultipleFilter) {
	ai::ConditionParser parser(_registry, "Filter(SelectEmpty,SelectHighestAggro)");
	const ai::ConditionPtr& c = parser.getCondition();
	ASSERT_NE(nullptr, c.get()) << parser.getError();
}

TEST_F(ParserTest, testSteer) {
	TestLoader loader(_registry);
	ai::TreeNodeParser parser(loader, "Steer{0.6,0.4}(GroupFlee{2},Wander{1})");
	const ai::TreeNodePtr& c = parser.getTreeNode();
	ASSERT_NE(nullptr, c.get()) << parser.getError();
}

TEST_F(ParserTest, testSteerGroupLeader) {
	TestLoader loader(_registry);
	ai::TreeNodeParser parser(loader, "Steer{0.6,0.4}(GroupFlee{2},FollowGroupLeader{1})");
	const ai::TreeNodePtr& c = parser.getTreeNode();
	ASSERT_NE(nullptr, c.get()) << parser.getError();
}

TEST_F(ParserTest, testSteerWithoutParam) {
	TestLoader loader(_registry);
	ai::TreeNodeParser parser(loader, "Steer(GroupFlee{2})");
	const ai::TreeNodePtr& c = parser.getTreeNode();
	ASSERT_NE(nullptr, c.get()) << parser.getError();
}

TEST_F(ParserTest, testSteerWanderWithoutAnyParam) {
	TestLoader loader(_registry);
	ai::TreeNodeParser parser(loader, "Steer(Wander)");
	const ai::TreeNodePtr& c = parser.getTreeNode();
	ASSERT_NE(nullptr, c.get()) << parser.getError();
}
