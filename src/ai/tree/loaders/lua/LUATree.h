#pragma once

#include <string>

namespace ai {

class ITreeLoader;
class LUANode;
class IAIFactory;

class LUATree {
private:
	std::string _name;
	ITreeLoader* _ctx;
	LUANode* _root;
	bool _subtree;
public:
	LUATree(const std::string& name, ITreeLoader* ctx, bool subtree) :
			_name(name), _ctx(ctx), _root(nullptr), _subtree(subtree) {
	}

	const IAIFactory& getAIFactory() const;

	bool setRoot(LUANode* root);

	inline bool isSubtree() const {
		return _subtree;
	}

	inline ITreeLoader& getTreeLoader() {
		return *_ctx;
	}

	inline const std::string& getName() const {
		return _name;
	}

	inline LUANode* getRoot() const {
		return _root;
	}
};

}
