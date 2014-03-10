#pragma once

#include <tree/TreeNode.h>

namespace ai {

#define SELECTOR_CLASS(NodeName) \
	NodeName(const std::string& name, ConditionPtr condition) : \
		Selector(name, condition) { \
	} \
	NodeName(const std::string& name) : \
		Selector(name) { \
	} \
	virtual ~NodeName() { \
	} \
	\
	NODE_FACTORY

/**
 * http://aigamedev.com/open/article/selector/
 */
class Selector: public TreeNode {
public:
	NODE_CLASS(Selector);

	void getChildrenState(const AI& entity, std::vector<bool>& active) const override {
		int n = 0;
		for (TreeNodes::const_iterator i = _children.begin(); i != _children.end(); ++i, ++n)
			active.push_back(getSelectorState(entity) == n);
	}
};

}
