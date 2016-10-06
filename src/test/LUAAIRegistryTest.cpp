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
	;
}

class LUAAIRegistryTest: public TestSuite {
protected:
	ai::LUAAIRegistry _registry;
	const ai::CharacterId _id = 1;
	ai::ICharacterPtr _chr = std::make_shared<TestEntity>(_id);
	const ai::TreeNodeFactoryContext ctx = ai::TreeNodeFactoryContext("TreeNodeName", "", ai::True::get());

	void SetUp() override {
		TestSuite::SetUp();
		ASSERT_TRUE(_registry.init()) << "Failed to initialize the lua ai registry";
		ASSERT_TRUE(_registry.evaluate(LUANODE, strlen(LUANODE))) << "Failed to load lua script:\n" << LUANODE;
	}

	void TearDown() override {
		TestSuite::TearDown();
		_registry.shutdown();
	}

	void testNode(const char* nodeName, ai::TreeNodeStatus status) {
		const ai::TreeNodePtr& node = _registry.createNode(nodeName, ctx);
		ASSERT_TRUE((bool)node) << "Could not create lua provided node '" << nodeName << "'";
		const ai::AIPtr& ai = std::make_shared<ai::AI>(node);
		ai->setCharacter(_chr);
		const ai::TreeNodeStatus executionStatus = node->execute(ai, 1L);
		ASSERT_EQ(status, executionStatus) << "Lua script returned an unexpected TreeNodeStatus value";
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

TEST_F(LUAAIRegistryTest, testCreateInvalidNode) {
	const ai::TreeNodePtr& node = _registry.createNode("ThisNameDoesNotExist", ctx);
	ASSERT_FALSE((bool)node) << "Created a node for a type that isn't defined";
}
