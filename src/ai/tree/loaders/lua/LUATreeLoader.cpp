#include <tree/loaders/lua/LUATreeLoader.h>
#include "LUA.h"
#include "LUAFunctions.h"

namespace ai {

#ifdef DEBUG
#define checkStack(state) StackChecker(state)
#else
#define checkStack(state)
#endif

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
		return false;
	}

	// loads all the trees
	_lua->newGlobalData<LUATreeLoader>("Loader", this);
	if (!_lua->execute("init")) {
		_error = _lua->getError();
		return false;
	}

	if (_treeMap.empty()) {
		_error = "no behaviour trees specified";
		return false;
	}
	return true;
}

void LUATreeLoader::getTrees(std::vector<std::string>& trees) const {
	for (TreeMap::const_iterator it = _treeMap.begin(); it != _treeMap.end(); ++it) {
		trees.push_back(it->first);
	}
}

bool LUATreeLoader::addTree(const std::string& name, TreeNodePtr& root) {
	if (!root) {
		_error = "Empty behaviour tree with id " + name + " given";
		return false;
	}
	TreeMap::const_iterator i = _treeMap.find(name);
	if (i != _treeMap.end()) {
		_error = "Behaviour tree with id " + name + " already exists";
		return false;
	}
	_treeMap.insert(std::make_pair(name, root));
	return true;
}

TreeNodePtr LUATreeLoader::load(const std::string &name) {
	TreeMap::const_iterator i = _treeMap.find(name);
	if (i != _treeMap.end())
		return i->second;
	_error = "No behaviour tree with id " + name + " found";
	return TreeNodePtr();
}

const std::string& LUATreeLoader::getError() const {
	const std::string& luaerror = _lua->getError();
	if (luaerror.empty())
		return _error;
	return luaerror;
}

}
