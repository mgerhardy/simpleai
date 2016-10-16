#include "LUAAIRegistryTest.h"

namespace {
static const char *LUANODE = ""
	// TODO: add the other lua bindings to this test
	"local luatest = REGISTRY.createNode(\"LuaTest\")\n"
	"function luatest:execute(ai, deltaMillis)\n"
	"	--print(\"LuaTest node execute called with parameters: ai=[\"..tostring(ai)..\"], deltaMillis=[\"..tostring(deltaMillis)..\"]\")\n"
	"	local chr = ai:character()\n"
	"	local pos = chr:position()\n"
	"	pos.x = 1.0\n"
	"	local zone = ai:zone()\n"
	"	if zone == nil then\n"
	"		print(\"error: ai has no zone assigned\")\n"
	"		return FAILED\n"
	"	end\n"
	"	local aiFromZone = zone:ai(chr:id())\n"
	"	if aiFromZone == nil then\n"
	"		print(\"error: could not get ai from zone with id \" .. chr:id())\n"
	"		return FAILED\n"
	"	end\n"
	"	local aggroMgr = ai:aggroMgr()\n"
	"	local aggroVal = aggroMgr:addAggro(2, 1.0)\n"
	"	if aggroVal ~= 1.0 then\n"
	"		print(\"error: expected aggroVal was 1.0 - but found was \" .. aggroVal)\n"
	"		return FAILED\n"
	"	end\n"
	"	local entries = aggroMgr:entries()\n"
	"	id, val = aggroMgr:highestEntry()\n"
	"	if id ~= 2 then\n"
	"		print(\"error: expected id was 2 - but found was \" .. id)\n"
	"		return FAILED\n"
	"	end\n"
	"	if val ~= aggroVal then\n"
	"		print(\"error: expected value was \" .. aggroVal .. \" - but found was \" .. val)\n"
	"		return FAILED\n"
	"	end\n"
	"	chr:setAttribute(\"Key\", \"Value\")\n"
	"	if chr:attributes()[\"Key\"] ~= \"Value\" then\n"
	"		print(\"error: expected attribute with Key is Value - but found was \" .. chr:attributes()[\"Key\"])\n"
	"		return FAILED\n"
	"	end\n"
	"	--[[\n"
	"	print(\"id: \" .. ai:id())\n"
	"	print(\"id: \" .. chr:id())\n"
	"	print(\"time: \" .. ai:time())\n"
	"	print(\"haszone: \" .. tostring(ai:hasZone()))\n"
	"	if ai:hasZone() then\n"
	"		print(\"zone: \" .. tostring(ai:zone()))\n"
	"	end\n"
	"	print(\"aggroentries: \" .. tostring(entries))\n"
	"	print(\"attributes: \" .. tostring(chr:attributes()))\n"
	"	print(\"position: \" .. tostring(pos))\n"
	"	print(\"position x: \" .. pos.x)\n"
	"	print(\"position y: \" .. pos.y)\n"
	"	print(\"position z: \" .. pos.z)\n"
	"	print(\"character: \" .. tostring(chr))\n"
	"	print(\"aggromgr: \" .. tostring(aggroMgr))\n"
	"	--]]\n"
	"	return FINISHED\n"
	"end\n"
	"local luatest2 = REGISTRY.createNode(\"LuaTest2\")\n"
	"function luatest2:execute(ai, deltaMillis)\n"
	"	--print(\"LuaTest2 node execute called with parameters: ai=[\"..tostring(ai)..\"], deltaMillis=[\"..tostring(deltaMillis)..\"]\")\n"
	"	return RUNNING\n"
	"end\n"
	"-- ensure we have a name clash here\n"
	"local luaconditiontest = REGISTRY.createCondition(\"LuaTest\")\n"
	"function luaconditiontest:evaluate(ai)\n"
	"	--print(\"LuaTest condition evaluate called with parameter: ai=[\"..tostring(ai)..\"]\")\n"
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
	"local luasteeringtest = REGISTRY.createSteering(\"LuaSteeringTest\")\n"
	"function luasteeringtest:execute(ai, speed)\n"
	"	return 0.0, 1.0, 0.0, 0.6\n"
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
	const ai::SteeringFactoryContext ctxSteering = ai::SteeringFactoryContext("");

	void SetUp() override {
		TestSuite::SetUp();
		ASSERT_TRUE(_registry.init()) << "Failed to initialize the lua ai registry";
		ASSERT_TRUE(_registry.evaluate(LUANODE, strlen(LUANODE))) << "Failed to load lua script:\n" << LUANODE;
	}

	void TearDown() override {
		TestSuite::TearDown();
		_registry.shutdown();
	}

	void testSteering(const char* steeringName, int n = 1) {
		SCOPED_TRACE(steeringName);
		const ai::SteeringPtr& steering = _registry.createSteering(steeringName, ctxSteering);
		ASSERT_TRUE((bool)steering) << "Could not create lua provided steering";
		const ai::AIPtr& ai = std::make_shared<ai::AI>(ai::TreeNodePtr());
		ai->setCharacter(_chr);
		for (int i = 0; i < n; ++i) {
			steering->execute(ai, 1.0f);
		}
		ASSERT_EQ(1, steering.use_count()) << "Someone is still referencing the LUASteering";
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
		ai::Zone zone("TestNode");
		const ai::TreeNodePtr& node = _registry.createNode(nodeName, ctx);
		ASSERT_TRUE((bool)node) << "Could not create lua provided node '" << nodeName << "'";
		const ai::AIPtr& ai = std::make_shared<ai::AI>(node);
		ai->setCharacter(_chr);
		ASSERT_TRUE(zone.addAI(ai));
		zone.update(1l);
		for (int i = 0; i < n; ++i) {
			const ai::TreeNodeStatus executionStatus = node->execute(ai, 1L);
			ASSERT_EQ(status, executionStatus) << "Lua script returned an unexpected TreeNodeStatus value";
		}
		ASSERT_TRUE(zone.removeAI(ai));
		zone.update(1l);
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

TEST_F(LUAAIRegistryTest, testSteeringEmpty) {
	testSteering("LuaSteeringTest");
}
