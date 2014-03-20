#include "ParserTest.h"
#include <conditions/ConditionParser.h>
#include <tree/TreeNodeParser.h>

class ParserTest: public TestSuite {
};

TEST_F(ParserTest, testParseConditionSimple) {
	ai::ConditionParser parser(_registry, "HasEnemies");
	const ai::ConditionPtr& c = parser.getCondition();
	ASSERT_TRUE(c) << parser.getError();
}

TEST_F(ParserTest, testParseConditionNot) {
	ai::ConditionParser parser(_registry, "Not(HasEnemies)");
	const ai::ConditionPtr& c = parser.getCondition();
	ASSERT_TRUE(c) << parser.getError();
}

TEST_F(ParserTest, testParseConditionAndNot) {
	ai::ConditionParser parser(_registry, "And(Not(HasEnemies),True)");
	const ai::ConditionPtr& c = parser.getCondition();
	ASSERT_TRUE(c) << parser.getError();
}

TEST_F(ParserTest, testParseConditionAndNotInnerParam) {
	ai::ConditionParser parser(_registry, "And(Not(HasEnemies{3}),True)");
	const ai::ConditionPtr& c = parser.getCondition();
	ASSERT_TRUE(c) << parser.getError();
}

TEST_F(ParserTest, testParseConditionAndNotInnerOuterParam) {
	ai::ConditionParser parser(_registry, "And(Not{3}(HasEnemies{3}),True)");
	const ai::ConditionPtr& c = parser.getCondition();
	ASSERT_TRUE(c) << parser.getError();
}

TEST_F(ParserTest, testParseConditionParmEverywhere) {
	ai::ConditionParser parser(_registry, "And{1}(Not{3}(HasEnemies{3}),True{1})");
	const ai::ConditionPtr& c = parser.getCondition();
	ASSERT_TRUE(c) << parser.getError();
}

TEST_F(ParserTest, testParseFail) {
	ai::ConditionParser parser(_registry, "And(Not(HasEnemies{3},True)");
	const ai::ConditionPtr& c = parser.getCondition();
	ASSERT_FALSE(c);
}

TEST_F(ParserTest, testParseTreeNode) {
	ai::TreeNodeParser parser(_registry, "Invert{1}");
	const ai::TreeNodePtr& c = parser.getTreeNode();
	ASSERT_TRUE(c) << parser.getError();
}

TEST_F(ParserTest, testParseIdleNode) {
	ai::TreeNodeParser parser(_registry, "Idle{1000}");
	const ai::TreeNodePtr& c = parser.getTreeNode();
	ASSERT_TRUE(c) << parser.getError();
}
