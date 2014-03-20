#pragma once

#include <tree/TreeNode.h>
#include <common/IParser.h>

namespace ai {

class IAIFactory;

class TreeNodeParser: public IParser {
private:
	const IAIFactory& _aiFactory;
	std::string _taskString;
public:
	TreeNodeParser(const IAIFactory& aiFactory, const std::string& taskString);
	virtual ~TreeNodeParser();

	TreeNodePtr getTreeNode(const std::string& name = "");
};

}
