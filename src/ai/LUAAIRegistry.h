/**
 * @file
 */
#pragma once

#include "AIRegistry.h"
#include "tree/LUATreeNode.h"

namespace ai {

/**
 * @brief Allows you to register lua @ai{TreeNode}s, @ai{Conditions} and so on.
 *
 * @see @ai{LUATreeNode}
 *
 * @par TreeNode
 * @code
 *
 * @encode
 */
class LUAAIRegistry : public AIRegistry {
protected:
	lua_State* _s = nullptr;

	using LuaNodeFactory = LUATreeNode::LUATreeNodeFactory;
	typedef std::shared_ptr<LuaNodeFactory> LUATreeNodeFactoryPtr;
	typedef std::map<std::string, LUATreeNodeFactoryPtr> FactoryMap;

	static FactoryMap& getFactories() {
		static FactoryMap _factories;
		return _factories;
	}

	static LUAAIRegistry* luaGetContext(lua_State * s) {
		lua_getglobal(s, "Registry");
		LUAAIRegistry* data = (LUAAIRegistry*) lua_touserdata(s, -1);
		lua_pop(s, 1);
		return data;
	}

	static LuaNodeFactory* luaGetFactoryContext(lua_State * s, int n) {
		return *(LuaNodeFactory **) lua_touserdata(s, n);
	}

	static AI* luaGetAIContext(lua_State * s, int n) {
		return *(AI **) luaL_checkudata(s, n, LUATreeNode::luaAIMetaName());
	}

	static int luaNodeEmptyExecute(lua_State* s) {
		const LuaNodeFactory* factory = luaGetFactoryContext(s, 1);
		return luaL_error(s, "There is no execute function set for node: %s", factory->type().c_str());
	}

	static int luaNodeToString(lua_State* s) {
		const LuaNodeFactory* factory = luaGetFactoryContext(s, 1);
		lua_pushfstring(s, "node: %s", factory->type().c_str());
		return 1;
	}

	static int luaNewIndex(lua_State* s) {
		// -3 is userdata
		lua_getmetatable(s, -3);
		// -3 is now the field string
		const char *field = luaL_checkstring(s, -3);
		// push -2 to -1 (the value)
		lua_pushvalue(s, -2);
		// set the value into the field
		lua_setfield(s, -2, field);
		lua_pop(s, 1);
		return 0;
	}

	static int luaAiGC(lua_State* s) {
		return 0;
	}

	static int luaAiToString(lua_State* s) {
		const AI* ai = luaGetAIContext(s, 1);
		TreeNodePtr treeNode = ai->getBehaviour();
		if (treeNode) {
			lua_pushfstring(s, "ai: %s", treeNode->getName().c_str());
		} else {
			lua_pushstring(s, "ai: no behaviour tree set");
		}
		return 1;
	}

	static const luaL_Reg* nodeFuncs() {
		static const luaL_Reg nodes[] = {
			{"execute", luaNodeEmptyExecute},
			{"__tostring", luaNodeToString},
			{"__newindex", luaNewIndex},
			{nullptr, nullptr}
		};
		return nodes;
	}

	/**
	 * @brief Create a new lua @ai{TreeNode}
	 *
	 * @par lua parameters: #1 name of the node
	 * @note you have to specify an @c execute method that accepts two parameters in your lua code. E.g. do it like this:
	 * @code
	 * local luatest = REGISTRY.createNode(\"LuaTest\")
	 " function luatest:execute(ai, deltaMillis)
	 "    return FAILED\n"
	 " end
	 * @endcode
	 */
	static int luaCreateNode(lua_State* s) {
		LUAAIRegistry* r = luaGetContext(s);
		const std::string type = luaL_checkstring(s, -1);
		const LUATreeNodeFactoryPtr& factory = std::make_shared<LuaNodeFactory>(s, type);
		const bool inserted = r->registerNodeFactory(type, *factory);
		if (!inserted) {
			luaL_error(s, "%s is already registered", type.c_str());
			return 0;
		}

		LuaNodeFactory ** udata = (LuaNodeFactory**) lua_newuserdata(s, sizeof(LuaNodeFactory*));
		// make global
		lua_setfield(s, LUA_REGISTRYINDEX, type.c_str());
		// put back onto stack
		lua_getfield(s, LUA_REGISTRYINDEX, type.c_str());

		// setup meta table - create a new one manually, otherwise we aren't
		// able to override the execute function on a per node base. Also
		// this 'metatable' must not be in the global registry.
		*udata = factory.get();
		lua_createtable(s, 0, 2);
		lua_pushvalue(s, -1);
		lua_setfield(s, -2, "__index");
		lua_pushstring(s, "node");
		lua_setfield(s, -2, "__name");
		luaL_setfuncs(s, nodeFuncs(), 0);
		lua_setmetatable(s, -2);

		// TODO: locking
		getFactories().emplace(type, factory);

		return 1;
	}

public:
	LUAAIRegistry() {
		init();
	}

	/**
	 * @brief Access to the lua state.
	 */
	lua_State* getLuaState() {
		return _s;
	}

	/**
	 * @see shutdown()
	 */
	bool init() {
		if (_s != nullptr) {
			return true;
		}
		_s = luaL_newstate();

		lua_atpanic(_s, [] (lua_State* L) {
			ai_log_error("Lua panic. Error message: %s", (lua_isnil(L, -1) ? "" : lua_tostring(L, -1)));
			return 0;
		});
		lua_gc(_s, LUA_GCSTOP, 0);
		luaL_openlibs(_s);

		luaL_Reg registryFuncs[] = {
			{"createNode", luaCreateNode},
			{nullptr, nullptr}
		};

		luaL_newmetatable(_s, "META_REGISTRY");
		lua_pushvalue(_s, -1);
		lua_setfield(_s, -2, "__index");
		luaL_setfuncs(_s, registryFuncs, 0);
		lua_setglobal(_s, "REGISTRY");

		lua_pushlightuserdata(_s, this);
		lua_setglobal(_s, "Registry");

		luaL_newmetatable(_s, "node");
		// assign the metatable to __index
		lua_pushvalue(_s, -1);
		lua_setfield(_s, -2, "__index");
		luaL_setfuncs(_s, nodeFuncs(), 0);

		luaL_Reg aiFuncs[] = {
			// TODO: make this extensible from outside
			{"__tostring", luaAiToString},
			{"__gc", luaAiGC},
			{nullptr, nullptr}
		};

		luaL_newmetatable(_s, LUATreeNode::luaAIMetaName());
		// assign the metatable to __index
		lua_pushvalue(_s, -1);
		lua_setfield(_s, -2, "__index");
		luaL_setfuncs(_s, aiFuncs, 0);

		const char* script = ""
			"UNKNOWN, CANNOTEXECUTE, RUNNING, FINISHED, FAILED, EXCEPTION = 0, 1, 2, 3, 4, 5\n";

		if (luaL_loadbufferx(_s, script, strlen(script), "", nullptr) || lua_pcall(_s, 0, 0, 0)) {
			ai_log_error("%s", lua_tostring(_s, -1));
			lua_pop(_s, 1);
			return false;
		}
		return true;
	}

	/**
	 * @see init()
	 */
	void shutdown() {
		getFactories().clear();
		if (_s != nullptr) {
			lua_close(_s);
			_s = nullptr;
		}
	}

	~LUAAIRegistry() {
		shutdown();
	}

	inline bool evaluate(const std::string& str) {
		return evaluate(str.c_str(), str.length());
	}

	/**
	 * @brief Load your lua scripts into the lua state of the registry.
	 * This can be called multiple times to e.g. load multiple files.
	 * @return @c true if the lua script was loaded, @c false otherwise
	 * @note you have to call init() before
	 */
	bool evaluate(const char* luaBuffer, size_t size) {
		if (_s == nullptr) {
			return false;
		}
		if (luaL_loadbufferx(_s, luaBuffer, size, "", nullptr) || lua_pcall(_s, 0, 0, 0)) {
			ai_log_error("%s", lua_tostring(_s, -1));
			lua_pop(_s, 1);
			return false;
		}
		return true;
	}
};

}
