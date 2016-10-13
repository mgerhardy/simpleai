#include "LUAAIRegistryTest.h"

namespace {
static const char *LUANODE = ""
	"local luatest = REGISTRY.createNode(\"LuaTest\")\n"
	"function luatest:execute(ai, deltaMillis)\n"
	"	print(\"LuaTest node execute called with parameters: ai=[\"..tostring(ai)..\"], deltaMillis=[\"..tostring(deltaMillis)..\"]\")\n"
	"	return FINISHED\n"
	"end\n"
	"local luatest2 = REGISTRY.createNode(\"LuaTest2\")\n"
	"function luatest2:execute(ai, deltaMillis)\n"
	"	print(\"LuaTest2 node execute called with parameters: ai=[\"..tostring(ai)..\"], deltaMillis=[\"..tostring(deltaMillis)..\"]\")\n"
	"	return RUNNING\n"
	"end\n"
	"-- ensure we have a name clash here\n"
	"local luaconditiontest = REGISTRY.createCondition(\"LuaTest\")\n"
	"function luaconditiontest:evaluate(ai)\n"
	"	print(\"LuaTest condition evaluate called with parameter: ai=[\"..tostring(ai)..\"]\")\n"
	"	return true\n"
	"end\n"
	"local luaconditiontesttrue = REGISTRY.createCondition(\"LuaTestTrue\")\n"
	"function luaconditiontesttrue:evaluate(ai)\n"
	"	return true\n"
	"end\n"
	"local luaconditiontestfalse = REGISTRY.createCondition(\"LuaTestFalse\")\n"
	"function luaconditiontestfalse:evaluate(ai)\n"
	"	return false\n"
	"end\n"
	"local luafiltertest = REGISTRY.createFilter(\"LuaFilterTest\")\n"
	"function luafiltertest:filter(ai)\n"
	"end\n"
	;
}

class LUAAIRegistryTest: public TestSuite {
protected:
	ai::LUAAIRegistry _registry;
	const ai::CharacterId _id = 1;
	ai::ICharacterPtr _chr = std::make_shared<TestEntity>(_id);
	const ai::ConditionFactoryContext ctxCondition = ai::ConditionFactoryContext("");
	const ai::FilterFactoryContext ctxFilter = ai::FilterFactoryContext("");

	void SetUp() override {
		TestSuite::SetUp();
		ASSERT_TRUE(_registry.init()) << "Failed to initialize the lua ai registry";
		ASSERT_TRUE(_registry.evaluate(LUANODE, strlen(LUANODE))) << "Failed to load lua script:\n" << LUANODE;
	}

	void TearDown() override {
		TestSuite::TearDown();
		_registry.shutdown();
	}

	void testFilter(const char* filterName, int n = 1) {
		SCOPED_TRACE(filterName);
		const ai::FilterPtr& filter = _registry.createFilter(filterName, ctxFilter);
		ASSERT_TRUE((bool)filter) << "Could not create lua provided filter";
		const ai::AIPtr& ai = std::make_shared<ai::AI>(ai::TreeNodePtr());
		ai->setCharacter(_chr);
		for (int i = 0; i < n; ++i) {
			filter->filter(ai);
		}
		ASSERT_EQ(1, filter.use_count()) << "Someone is still referencing the LUAFilter";
	}

	void testCondition(const char* conditionName, bool expectedReturnValue, int n = 1) {
		SCOPED_TRACE(conditionName);
		const ai::ConditionPtr& condition = _registry.createCondition(conditionName, ctxCondition);
		ASSERT_TRUE((bool)condition) << "Could not create lua provided condition";
		const ai::AIPtr& ai = std::make_shared<ai::AI>(ai::TreeNodePtr());
		ai->setCharacter(_chr);
		for (int i = 0; i < n; ++i) {
			ASSERT_EQ(expectedReturnValue, condition->evaluate(ai));
		}
		ASSERT_EQ(1, condition.use_count()) << "Someone is still referencing the LUACondition";
	}

	void testNode(const char* nodeName, ai::TreeNodeStatus status, int n = 1) {
		const ai::TreeNodeFactoryContext ctx = ai::TreeNodeFactoryContext("TreeNodeName", "", ai::True::get());
		return testNode(nodeName, status, ctx, n);
	}

	void testNode(const char* nodeName, ai::TreeNodeStatus status, const ai::TreeNodeFactoryContext &ctx, int n = 1) {
		SCOPED_TRACE(nodeName);
		const ai::TreeNodePtr& node = _registry.createNode(nodeName, ctx);
		ASSERT_TRUE((bool)node) << "Could not create lua provided node '" << nodeName << "'";
		const ai::AIPtr& ai = std::make_shared<ai::AI>(node);
		ai->setCharacter(_chr);
		for (int i = 0; i < n; ++i) {
			const ai::TreeNodeStatus executionStatus = node->execute(ai, 1L);
			ASSERT_EQ(status, executionStatus) << "Lua script returned an unexpected TreeNodeStatus value";
		}
		ai->setBehaviour(ai::TreeNodePtr());
		ASSERT_EQ(1, node.use_count()) << "Someone is still referencing the LUATreeNode";
		ASSERT_EQ(1, ai.use_count()) << "Someone is still referencing the AI instance";
	}
};

TEST_F(LUAAIRegistryTest, testLuaNode1) {
	testNode("LuaTest", ai::TreeNodeStatus::FINISHED);
}

TEST_F(LUAAIRegistryTest, testLuaNode2) {
	testNode("LuaTest2", ai::TreeNodeStatus::RUNNING);
}

TEST_F(LUAAIRegistryTest, testLuaNode2_100) {
	testNode("LuaTest2", ai::TreeNodeStatus::RUNNING, 100);
}

TEST_F(LUAAIRegistryTest, testCreateInvalidNode) {
	const ai::TreeNodeFactoryContext ctx = ai::TreeNodeFactoryContext("TreeNodeName", "", ai::True::get());
	const ai::TreeNodePtr& node = _registry.createNode("ThisNameDoesNotExist", ctx);
	ASSERT_FALSE((bool)node) << "Created a node for a type that isn't defined";
}

TEST_F(LUAAIRegistryTest, testConditionEvaluationTrue) {
	testCondition("LuaTestTrue", true);
}

TEST_F(LUAAIRegistryTest, testConditionEvaluationTrue_100) {
	testCondition("LuaTestTrue", true, 100);
}

TEST_F(LUAAIRegistryTest, testConditionEvaluationFalse) {
	testCondition("LuaTestFalse", false);
}

TEST_F(LUAAIRegistryTest, testFilterEmpty) {
	testFilter("LuaFilterTest");
}

TEST_F(LUAAIRegistryTest, testFilter_100) {
	testFilter("LuaFilterTest", 100);
}
