#include "TreeNodeParser.h"

namespace ai {

TreeNodeParser::TreeNodeParser(const IAIFactory& aiFactory, const std::string& taskString) :
		IParser(), _aiFactory(aiFactory) {
	_taskString = eraseAllSpaces(taskString);
}

TreeNodeParser::~TreeNodeParser() {
}

TreeNodePtr TreeNodeParser::getTreeNode(const std::string& name) {
	std::string nodeType;
	const std::string& parameters = getBetween(_taskString, "{", "}");
	std::size_t n = _taskString.find("{");
	if (n == std::string::npos)
		n = _taskString.find("(");
	if (n != std::string::npos) {
		nodeType = _taskString.substr(0, n);
	} else {
		nodeType = _taskString;
	}
	const TreeNodeFactoryContext factoryCtx(name.empty() ? nodeType : name, parameters, True::get());
	return _aiFactory.createNode(nodeType, factoryCtx);
}
}
