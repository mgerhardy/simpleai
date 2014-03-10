#pragma once

#include <common/Compiler.h>
#include <common/Pointers.h>
#include <string>
#include <vector>

namespace ai {

class IAIFactory;
class TreeNode;
typedef SharedPtr<TreeNode> TreeNodePtr;

class ITreeLoader {
protected:
	const IAIFactory& _aiFactory;
public:
	ITreeLoader(const IAIFactory& aiFactory) :
			_aiFactory(aiFactory) {
	}

	virtual ~ITreeLoader() {
	}

	virtual TreeNodePtr load(const std::string &name) = 0;
	virtual void getTrees(std::vector<std::string>& trees) const = 0;

	inline const IAIFactory& getAIFactory() const {
		return _aiFactory;
	}
};

}
