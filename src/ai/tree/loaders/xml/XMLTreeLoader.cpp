#include "XMLTreeLoader.h"
#include <tinyxml2.h>
#include "conditions/ConditionParser.h"
#include "tree/TreeNodeParser.h"

namespace ai {

namespace {

TreeNodePtr loadSubTreeFromXML (const IAIFactory& aiFactory, tinyxml2::XMLElement* e) {
	if (e == nullptr) {
		return TreeNodePtr();
	}

	const char *name = e->Attribute("name", nullptr);
	if (name == nullptr) {
		return TreeNodePtr();
	}

	const char *type = e->Attribute("type", nullptr);
	if (type == nullptr) {
		return TreeNodePtr();
	}

	TreeNodeParser nodeParser(aiFactory, type);
	const TreeNodePtr& node = nodeParser.getTreeNode(name);
	if (!node) {
		return TreeNodePtr();
	}

	const char *condition = e->Attribute("condition", nullptr);
	if (condition == nullptr) {
		condition = "True";
	}

	ConditionParser conditionParser(aiFactory, condition);
	const ConditionPtr& conditionPtr = conditionParser.getCondition();
	if (!conditionPtr.get()) {
		return TreeNodePtr();
	}

	node->setCondition(conditionPtr);
	return node;
}

TreeNodePtr loadTreeFromXML (const IAIFactory& aiFactory, tinyxml2::XMLElement* rootNode) {
	TreeNodePtr root = loadSubTreeFromXML(aiFactory, rootNode);
	if (!root.get())
		return root;
	for (tinyxml2::XMLNode* node = rootNode->FirstChild(); node; node = node->NextSibling()) {
		tinyxml2::XMLElement* e = node->ToElement();
		const TreeNodePtr& child = loadSubTreeFromXML(aiFactory, e);
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
	tinyxml2::XMLElement* rootNode = doc.FirstChildElement("behaviours");
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
		if ("behaviour" != treeNodeName) {
			_error = "unexpected node name - expected 'behaviour' - got " + treeNodeName;
			continue;
		}
		const char *name = e->Attribute("name");
		if (name == nullptr) {
			_error = "node 'behaviour' does not have a 'name' attribute";
			continue;
		}
		tinyxml2::XMLNode* rootXMLNode = e->FirstChild();
		if (rootXMLNode == nullptr)
			continue;
		const TreeNodePtr& root = loadTreeFromXML(_aiFactory, rootXMLNode->ToElement());
		if (root.get() == nullptr) {
			_error = "could not create the root node";
			continue;
		}
		addTree(name, root);
	}
	if (status != tinyxml2::XML_NO_ERROR)
		return false;
	return !_treeMap.empty();
}

}
