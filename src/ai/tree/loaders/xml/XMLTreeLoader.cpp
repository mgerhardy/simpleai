#include "XMLTreeLoader.h"
#include <tinyxml2.h>
#include "conditions/ConditionParser.h"
#include "tree/TreeNodeParser.h"

namespace ai {

namespace {

TreeNodePtr loadSubTreeFromXML (std::string _error, ITreeLoader& treeLoader, tinyxml2::XMLElement* e) {
	if (e == nullptr) {
		_error = "Missing xml element";
		return TreeNodePtr();
	}

	const char *name = e->Attribute("name", nullptr);
	if (name == nullptr) {
		_error = "No name attribute";
		return TreeNodePtr();
	}

	const char *type = e->Attribute("type", nullptr);
	if (type == nullptr) {
		_error = "No type attribute";
		return TreeNodePtr();
	}

	TreeNodeParser nodeParser(treeLoader, type);
	const TreeNodePtr& node = nodeParser.getTreeNode(name);
	if (!node) {
		_error = "Could not create node for " + std::string(name);
		return TreeNodePtr();
	}

	const char *condition = e->Attribute("condition", nullptr);
	if (condition == nullptr) {
		condition = "True";
	}

	ConditionParser conditionParser(treeLoader.getAIFactory(), condition);
	const ConditionPtr& conditionPtr = conditionParser.getCondition();
	if (!conditionPtr.get()) {
		_error = "Could not parse condition";
		return TreeNodePtr();
	}

	node->setCondition(conditionPtr);
	return node;
}

TreeNodePtr loadTreeFromXML (std::string _error, ITreeLoader& treeLoader, tinyxml2::XMLElement* rootNode) {
	TreeNodePtr root = loadSubTreeFromXML(_error, treeLoader, rootNode);
	if (!root.get())
		return root;
	for (tinyxml2::XMLNode* node = rootNode->FirstChild(); node; node = node->NextSibling()) {
		tinyxml2::XMLElement* e = node->ToElement();
		const TreeNodePtr& child = loadSubTreeFromXML(_error, treeLoader, e);
		if (child.get() == nullptr)
			continue;
		root->addChild(child);
	}
	return root;
}

}

XMLTreeLoader::XMLTreeLoader(const IAIFactory& aiFactory) :
		ITreeLoader(aiFactory) {
}

bool XMLTreeLoader::init(const std::string& xmlData) {
	tinyxml2::XMLDocument doc(false);
	const int status = doc.Parse(xmlData.c_str());
	tinyxml2::XMLElement* rootNode = doc.FirstChildElement("trees");
	if (rootNode == nullptr)
		return false;
	for (tinyxml2::XMLNode* node = rootNode->FirstChild(); node; node = node->NextSibling()) {
		tinyxml2::XMLElement* e = node->ToElement();
		if (e == nullptr) {
			_error = "unexpected node type";
			continue;
		}
		if (e->Name() == nullptr) {
			_error = "expected node name but didn't find one";
			continue;
		}
		const std::string treeNodeName(e->Name());
		if ("tree" == treeNodeName || "subtree" == treeNodeName) {
			const char *name = e->Attribute("name");
			if (name == nullptr) {
				_error = "node 'behaviour' does not have a 'name' attribute";
				continue;
			}
			tinyxml2::XMLNode* rootXMLNode = e->FirstChild();
			if (rootXMLNode == nullptr)
				continue;
			const TreeNodePtr& root = loadTreeFromXML(_error, *this, rootXMLNode->ToElement());
			if (root.get() == nullptr) {
				if (_error.empty())
					_error = "could not create the root node";
				continue;
			}
			addTree(name, root);
		} else {
			_error = "unexpected node name - expected 'tree or subtree' - got " + treeNodeName;
			continue;
		}
	}
	if (status != tinyxml2::XML_NO_ERROR)
		return false;
	return _error.empty() && !_treeMap.empty();
}

}
