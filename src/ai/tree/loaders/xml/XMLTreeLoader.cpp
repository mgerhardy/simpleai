#include "XMLTreeLoader.h"
#include <tinyxml2.h>
#include "conditions/ConditionParser.h"
#include "tree/TreeNodeParser.h"

namespace ai {

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
			return false;
		}
		if (::strcmp("node", e->Name())) {
			_error = "unexpected node name";
			return false;
		}
		const char *type = e->Attribute("type");
		if (type == nullptr) {
			_error = "node has no type attribute";
			return false;
		}
	}
	/** @todo implement parsing */
	return status == tinyxml2::XML_NO_ERROR;
}

}
