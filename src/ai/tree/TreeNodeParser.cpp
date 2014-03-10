#include "TreeNodeParser.h"

namespace ai {

TreeNodeParser::TreeNodeParser(const IAIFactory& aiFactory, const std::string& taskString) :
		IParser(), _aiFactory(aiFactory) {
	_taskString = eraseAllSpaces(taskString);
}

TreeNodeParser::~TreeNodeParser() {
}

TreeNodePtr TreeNodeParser::getTreeNode() {
	std::string name;
	std::string parameters = getBetween(_taskString, "{", "}");
	std::size_t n = _taskString.find("{");
	if (n == std::string::npos)
		n = _taskString.find("(");
	if (n != std::string::npos) {
		name = _taskString.substr(0, n);
	} else {
		name = _taskString;
	}
	TreeNodeFactoryContext factoryCtx(name, parameters);
	return _aiFactory.createNode(name, factoryCtx);
}
}
