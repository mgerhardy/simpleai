#pragma once

#include <common/Compiler.h>
#include <common/Pointers.h>
#include <string>
#include <vector>
#include <map>

namespace ai {

class IAIFactory;
class TreeNode;
typedef SharedPtr<TreeNode> TreeNodePtr;

class ITreeLoader {
protected:
	const IAIFactory& _aiFactory;
	typedef std::map<std::string, TreeNodePtr> TreeMap;
	TreeMap _treeMap;
	std::string _error;
public:
	ITreeLoader(const IAIFactory& aiFactory) :
			_aiFactory(aiFactory) {
	}

	virtual ~ITreeLoader() {
	}

	inline const IAIFactory& getAIFactory() const {
		return _aiFactory;
	}

	void getTrees(std::vector<std::string>& trees) const {
		for (TreeMap::const_iterator it = _treeMap.begin(); it != _treeMap.end(); ++it) {
			trees.push_back(it->first);
		}
	}

	bool addTree(const std::string& name, TreeNodePtr& root) {
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

	/**
	 * @brief Loads on particular behaviour tree.
	 */
	TreeNodePtr load(const std::string &name) {
		TreeMap::const_iterator i = _treeMap.find(name);
		if (i != _treeMap.end())
			return i->second;
		_error = "No behaviour tree with id " + name + " found";
		return TreeNodePtr();
	}

	inline const std::string& getError() const {
		return _error;
	}
};

}
