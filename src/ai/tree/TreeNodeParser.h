#pragma once

#include "tree/TreeNode.h"
#include "common/IParser.h"

namespace ai {

class IAIFactory;

/**
 * @brief Transforms the string representation of a @c TreeNode with all its
 * parameters into a @c TreeNode instance.
 *
 * @c #NodeName{Parameters}
 * Parameters are by default optional - but that really depends on the
 * @c TreeNode implementation.
 */
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
