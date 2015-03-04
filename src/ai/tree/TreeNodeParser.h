#pragma once

#include "tree/TreeNode.h"
#include "common/IParser.h"

namespace ai {

class ITreeLoader;

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
	ITreeLoader& _treeLoader;
	std::string _taskString;

	void splitTasks(const std::string& string, std::vector<std::string>& tokens) const;
	SteeringPtr getSteering(const std::string& nodeName);
public:
	TreeNodeParser(ITreeLoader& treeLoader, const std::string& taskString);
	virtual ~TreeNodeParser();

	TreeNodePtr getTreeNode(const std::string& name = "");
};

}
