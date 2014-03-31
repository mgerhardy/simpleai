#include <tree/loaders/lua/LUATreeLoader.h>
#include "LUA.h"
#include "LUAFunctions.h"

namespace ai {

LUATreeLoader::LUATreeLoader(const IAIFactory& aiFactory) :
		ITreeLoader(aiFactory), _lua(new LUA()) {
	luaL_Reg createTree = { "createTree", luaMain_CreateTree };
	luaL_Reg eof = { nullptr, nullptr };
	luaL_Reg funcs[] = { createTree, eof };

	LUAType tree = _lua->registerType("Tree");
	tree.addFunction("createRoot", luaTree_CreateRoot);
	tree.addFunction("__gc", luaTree_GC);
	tree.addFunction("__tostring", luaTree_ToString);

	LUAType node = _lua->registerType("Node");
	node.addFunction("addNode", luaNode_AddNode);
	node.addFunction("setCondition", luaNode_SetCondition);
	node.addFunction("__gc", luaNode_GC);
	node.addFunction("__tostring", luaNode_ToString);

	_lua->reg("AI", funcs);
}

LUATreeLoader::~LUATreeLoader() {
	delete _lua;
}

bool LUATreeLoader::init(const std::string& luaString) {
	if (!_lua->load(luaString)) {
		_error = _lua->getError();
		return false;
	}

	// loads all the trees
	_lua->newGlobalData<LUATreeLoader>("Loader", this);
	if (!_lua->execute("init")) {
		_error = _lua->getError();
		return false;
	}

	if (_treeMap.empty()) {
		_error = "No behaviour trees specified";
		return false;
	}
	return true;
}

}
