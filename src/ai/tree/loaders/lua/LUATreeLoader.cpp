#include <tree/loaders/lua/LUATreeLoader.h>
#include "LUA.h"
#include "LUAFunctions.h"

namespace ai {

LUATreeLoader::LUATreeLoader(const IAIFactory& aiFactory) :
		ITreeLoader(aiFactory) {
}

bool LUATreeLoader::init(const std::string& luaString) {
	_error = "";
	_treeMap.clear();

	LUA lua;
	luaL_Reg createTree = { "createTree", luaMain_CreateTree };
	luaL_Reg eof = { nullptr, nullptr };
	luaL_Reg funcs[] = { createTree, eof };

	LUAType tree = lua.registerType("Tree");
	tree.addFunction("createRoot", luaTree_CreateRoot);
	tree.addFunction("getName", luaTree_GetName);
	tree.addFunction("__gc", luaTree_GC);
	tree.addFunction("__tostring", luaTree_ToString);

	LUAType node = lua.registerType("Node");
	node.addFunction("addNode", luaNode_AddNode);
	node.addFunction("getName", luaNode_GetName);
	node.addFunction("setCondition", luaNode_SetCondition);
	node.addFunction("__gc", luaNode_GC);
	node.addFunction("__tostring", luaNode_ToString);

	lua.reg("AI", funcs);

	if (!lua.load(luaString)) {
		_error = lua.getError();
		return false;
	}

	// loads all the trees
	lua.newGlobalData<LUATreeLoader>("Loader", this);
	if (!lua.execute("init")) {
		_error = lua.getError();
		return false;
	}

	if (_treeMap.empty()) {
		_error = "No behaviour trees specified";
		return false;
	}
	return true;
}

}
