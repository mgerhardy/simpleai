#pragma once

#include <common/Compiler.h>
#include <tree/loaders/ITreeLoader.h>
#include <string>
#include <map>
#include <vector>

struct LUA;

namespace ai {

class LUATreeLoader: public ai::ITreeLoader {
private:
	typedef std::map<std::string, TreeNodePtr> TreeMap;
	TreeMap _treeMap;
	LUA *_lua;
	std::string _error;
public:
	LUATreeLoader(const IAIFactory& aiFactory);
	bool init(const std::string& luaString);
	bool addTree(const std::string& name, TreeNodePtr& root);
	TreeNodePtr load(const std::string &name) override;
	void getTrees(std::vector<std::string>& trees) const override;

	const std::string& getError() const;
	virtual ~LUATreeLoader();
};

}
