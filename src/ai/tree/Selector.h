#pragma once

#include <tree/TreeNode.h>

namespace ai {

#define SELECTOR_CLASS(NodeName) \
	NodeName(const std::string& name, const std::string& parameters, const ConditionPtr& condition) : \
		Selector(name, parameters, condition) { \
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
	NODE_CLASS(Selector)

	virtual void getChildrenState(const AI& entity, std::vector<bool>& active) const override {
		int n = 0;
		int selectorState = getSelectorState(entity);
		for (TreeNodes::const_iterator i = _children.begin(); i != _children.end(); ++i, ++n)
			active.push_back(selectorState == n);
	}
};

}
